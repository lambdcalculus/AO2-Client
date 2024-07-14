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
#include "graphicsspriteitem.h"

#include <QGraphicsScene>
#include <QPainter>

#include <functional>

#include <modules/managers/scene_manager.h>

using namespace mk2;

GraphicsSpriteItem::GraphicsSpriteItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_player(new SpritePlayer)
{
  connect(m_player.get(), SIGNAL(size_changed(QSize)), this, SLOT(notify_size()));
  connect(m_player.get(), SIGNAL(current_frame_changed()), this, SLOT(notify_update()));
  connect(m_player.get(), SIGNAL(file_name_changed(QString)), this, SIGNAL(file_name_changed(QString)));
  connect(m_player.get(), SIGNAL(reader_changed()), this, SIGNAL(reader_changed()));
  connect(m_player.get(), SIGNAL(started()), this, SIGNAL(started()));
  connect(m_player.get(), SIGNAL(finished()), this, SIGNAL(finished()));
}

GraphicsSpriteItem::~GraphicsSpriteItem()
{}

SpritePlayer::ScalingMode GraphicsSpriteItem::get_scaling_mode() const
{
  return m_player->get_scaling_mode();
}

void GraphicsSpriteItem::set_scaling_mode(SpritePlayer::ScalingMode p_scaling_mode)
{
  m_player->set_scaling_mode(p_scaling_mode);
}

void GraphicsSpriteItem::set_center_mode(bool t_center)
{
  mCenterSprite = t_center;
}

void GraphicsSpriteItem::set_composition_mode(QPainter::CompositionMode t_composition)
{
  mCompoMode = t_composition;
}

QSizeF GraphicsSpriteItem::get_size() const
{
  return m_player->get_size();
}

void GraphicsSpriteItem::set_size(QSizeF p_size)
{
  m_player->set_size(p_size.toSize());
}

void GraphicsSpriteItem::set_play_once(bool p_enabled)
{
  m_player->set_play_once(p_enabled);
}

void GraphicsSpriteItem::set_mirror(bool p_enabled)
{
  m_player->set_mirror(p_enabled);
}

QString GraphicsSpriteItem::get_file_name() const
{
  return m_player->get_file_name();
}

void GraphicsSpriteItem::set_file_name(QString p_file_name)
{
  m_player->set_file_name(p_file_name);
}

QIODevice *GraphicsSpriteItem::get_device() const
{
  return m_player->get_device();
}

void GraphicsSpriteItem::set_device(QIODevice *p_device)
{
  m_player->set_device(p_device);
}

SpriteReader::ptr GraphicsSpriteItem::get_reader() const
{
  return m_player->get_reader();
}

void GraphicsSpriteItem::set_reader(SpriteReader::ptr p_reader)
{
  m_player->set_reader(p_reader);
}

SpritePlayer *GraphicsSpriteItem::get_player() const
{
  return m_player.get();
}

void GraphicsSpriteItem::start()
{
  m_player->start();
}

void GraphicsSpriteItem::restart()
{
  stop();
  start();
}

void GraphicsSpriteItem::setVerticalOffset(int t_offset)
{
  mVerticalVPOffset = -t_offset;
}

void GraphicsSpriteItem::setBackgroundScaling(double t_offset)
{
  //TO-DO
  //DRBackgroundSettings l_settings = SceneManager::get().getBackgroundSettings();
  //double l_minimum_scale = l_settings.mScaleMinimum;
  //double l_maxScale = l_settings.mScaleMax;
  //
  //double l_minimumPosition = l_settings.mPositionMinimum;
  //double l_maxPosition = l_settings.mPositionMaximum;

}

void GraphicsSpriteItem::setCurrentAnimation(DROAnimation *t_animation)
{
  mWidgetAnimation = t_animation;
}

void GraphicsSpriteItem::stop()
{
  m_player->stop();
}

bool GraphicsSpriteItem::is_valid() const
{
  return m_player->is_valid();
}

bool GraphicsSpriteItem::is_running() const
{
  return m_player->is_running();
}

QRectF GraphicsSpriteItem::boundingRect() const
{
  return QRectF(0, 0, m_player->get_size().width(), m_player->get_size().height());
}

void GraphicsSpriteItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);


  QImage l_image = m_player->get_current_frame();
  if (!l_image.isNull())
  {
    //if(mWidgetAnimation != nullptr)
    //{
    //  int lWidth = mWidgetAnimation->GetCurrentValue(eVarWidth);
    //  int lHeight = mWidgetAnimation->GetCurrentValue(eVarHeight);
    //  if(lWidth != 9999999 && lHeight != 9999999 && mWidgetAnimation->GetCurrentlyRunning())
    //  {
    //    l_image = l_image.scaled(QSize(lWidth, lHeight), Qt::IgnoreAspectRatio);
    //    update();
    //  }
    //}

    painter->save();
    painter->setCompositionMode(mCompoMode);
    // calculate center position
    QPointF l_horizontal_center(m_TransformX, m_TransformY);
    if(mCenterSprite)
    {
      if (auto *l_scene = scene())
      {
        const QPointF l_center = l_scene->sceneRect().center() - m_player->get_scaled_bounding_rect().center();
        l_horizontal_center.setX(l_horizontal_center.x() + l_center.x());
        l_horizontal_center.setY(l_horizontal_center.y() + mVerticalVPOffset);
      }
    }
    if(rotation() == 0)
    {
      painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
      painter->setRenderHint(QPainter::Antialiasing, false);
    }
    else
    {
      painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
      painter->setRenderHint(QPainter::Antialiasing, true);
    }

    painter->drawImage(l_horizontal_center, l_image);

    //painter->setPen(Qt::blue);
    //painter->drawRect(scene()->sceneRect());

    //painter->setPen(Qt::red);
    //painter->drawRect(m_player->get_scaled_bounding_rect());

    painter->restore();
  }
}

void GraphicsSpriteItem::notify_size()
{
  emit size_changed(QSizeF(m_player->get_size()));
}

void GraphicsSpriteItem::notify_update()
{
  update();
}
