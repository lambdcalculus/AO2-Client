/**************************************************************************
**
** mk2
** Copyright (C) 2022 Tricky Leifa
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Affero General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Affero General Public License for more details.
**
** You should have received a copy of the GNU Affero General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include "mk2/spriteviewer.h"
#include "mk2/spritedynamicreader.h"

#include <QFile>
#include <QResizeEvent>

using namespace mk2;

SpriteViewer::SpriteViewer(QWidget *parent)
    : QLabel{parent}
    , m_reader{new SpriteDynamicReader}
    , m_scaling_mode{StretchScaling}
    , m_resolved_scaling_mode{StretchScaling}
    , m_transform{Qt::SmoothTransformation}
    , m_running{false}
    , m_mirror{false}
    , m_play_once{false}
    , m_frame_count{0}
    , m_frame_number{0}
{
  setAlignment(Qt::AlignCenter);

  m_frame_timer.setTimerType(Qt::PreciseTimer);
  m_frame_timer.setSingleShot(true);

  m_repaint_timer.setInterval(200);
  m_repaint_timer.setSingleShot(true);

  connect(&m_frame_timer, SIGNAL(timeout()), this, SLOT(_p_paint_next_frame()));
  connect(&m_repaint_timer, SIGNAL(timeout()), this, SLOT(_p_paint_frame()));
}

SpriteViewer::~SpriteViewer()
{}

QString SpriteViewer::get_file_name() const
{
  return m_reader->get_file_name();
}

QIODevice *SpriteViewer::get_device() const
{
  return nullptr;
}

SpriteViewer::ScalingMode SpriteViewer::get_scaling_mode() const
{
  return m_scaling_mode;
}

void SpriteViewer::set_file_name(QString p_file_name)
{
  if (!p_file_name.isEmpty() && p_file_name == get_file_name())
  {
    return;
  }
  stop();
  m_reader->set_file_name(p_file_name);
  m_frame_count = m_reader->get_frame_count();
  emit file_name_changed(p_file_name);
}

void SpriteViewer::set_device(QIODevice *p_device)
{
  const QString l_prev_file_name = get_file_name();
  stop();
  m_reader->set_device(p_device);
  m_frame_count = m_reader->get_frame_count();
  const QString l_file_name = get_file_name();
  if (l_file_name != l_prev_file_name)
  {
    emit file_name_changed(l_file_name);
  }
}

void SpriteViewer::set_play_once(bool p_on)
{
  m_play_once = p_on;
}

void SpriteViewer::set_mirror(bool p_on)
{
  m_mirror = p_on;
}

void SpriteViewer::set_scaling_mode(ScalingMode scaling_mode)
{
  if (m_scaling_mode == scaling_mode)
  {
    return;
  }
  m_scaling_mode = scaling_mode;
  _p_resolve_scaling_mode();
  _p_paint_frame();
}

SpriteReader::ptr SpriteViewer::get_reader() const
{
  return m_reader;
}

void SpriteViewer::set_reader(SpriteReader::ptr p_reader)
{
  const QString l_prev_file_name = get_file_name();
  stop();
  if (p_reader == nullptr)
  {
    p_reader = SpriteReader::ptr(new SpriteDynamicReader);
  }
  m_reader = p_reader;
  m_frame_count = m_reader->get_frame_count();
  const QString l_file_name = get_file_name();
  if (l_file_name != l_prev_file_name)
  {
    emit file_name_changed(l_file_name);
  }
}

bool SpriteViewer::is_valid() const
{
  return m_reader->is_valid();
}

bool SpriteViewer::is_running() const
{
  return m_running;
}

void SpriteViewer::start()
{
  m_running = true;
  m_elapsed_timer.start();
  emit started();
  _p_resolve_scaling_mode();
  _p_paint_next_frame();
}

void SpriteViewer::restart()
{
  stop();
  start();
}

void SpriteViewer::stop()
{
  m_running = false;
  m_frame_timer.stop();
  m_frame_number = 0;
}

void SpriteViewer::resizeEvent(QResizeEvent *p_event)
{
  QLabel::resizeEvent(p_event);
  _p_resolve_scaling_mode();
  m_repaint_timer.start();
}

void SpriteViewer::_p_resolve_scaling_mode()
{
  m_resolved_scaling_mode = m_scaling_mode;

  const QSize p_size = size();
  const QSize p_image_size = m_reader->get_sprite_size();
  if (p_size == p_image_size || !p_image_size.isValid())
  {
    m_resolved_scaling_mode = NoScaling;
  }
  else if (m_resolved_scaling_mode == DynamicScaling)
  {
    const qreal l_width_factor = (qreal)qMax(p_image_size.width(), 1) / qMax(p_size.width(), 1);
    const qreal l_height_factor = (qreal)qMax(p_image_size.height(), 1) / qMax(p_size.height(), 1);

    const QSize l_by_width_size{
        int((qreal)p_image_size.width() / l_width_factor),
        int((qreal)p_image_size.height() / l_width_factor),
    };
    const QSize l_by_height_size{
        int((qreal)p_image_size.width() / l_height_factor),
        int((qreal)p_image_size.height() / l_height_factor),
    };

    if (l_by_width_size.height() >= p_size.height())
    {
      m_resolved_scaling_mode = WidthScaling;
    }
    else if (l_by_height_size.width() >= p_size.width())
    {
      m_resolved_scaling_mode = HeightScaling;
    }
    else
    {
      m_resolved_scaling_mode = StretchScaling;
    }
  }

  m_transform = Qt::SmoothTransformation;
  if (p_image_size.width() < p_size.width() || p_image_size.height() < p_size.height())
  {
    m_transform = Qt::FastTransformation;
  }
}

void SpriteViewer::_p_paint_next_frame()
{
  QElapsedTimer l_timer;
  l_timer.start();

  if (!is_valid())
  {
    if (m_running && m_play_once)
    {
      m_running = false;
      emit finished();
    }

    return;
  }

  if (!m_running)
  {
    return;
  }

  if (m_frame_number >= m_frame_count)
  {
    if (m_play_once)
    {
      m_running = false;
      emit finished();
      return;
    }

    if (m_frame_count > 1)
    {
      m_frame_number = 0;
    }
    else
    {
      return;
    }
  }

  const int l_current_frame_number = m_frame_number;
  m_current_frame = m_reader->get_frame(m_frame_number++);
  _p_paint_frame();

  const int l_next_delay = m_current_frame.delay - l_timer.elapsed();
  m_frame_timer.start(qMax(0, l_next_delay));
}

void SpriteViewer::_p_paint_frame()
{
  QPixmap l_image = m_current_frame.image;

  if (!l_image.isNull())
  {
    switch (m_resolved_scaling_mode)
    {
      case NoScaling:
        [[fallthrough]];
      default:
        break;

      case StretchScaling:
        l_image = l_image.scaled(size(), Qt::IgnoreAspectRatio, m_transform);
        break;

      case WidthScaling:
        l_image = l_image.scaledToWidth(width(), m_transform);
        break;

      case HeightScaling:
        l_image = l_image.scaledToHeight(height(), m_transform);
        break;
    }
  }

  // slow operation...
  if (m_mirror)
  {
    l_image = QPixmap::fromImage(l_image.toImage().mirrored(true, false));
  }

  setPixmap(l_image);
}
