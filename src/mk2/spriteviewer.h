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

#pragma once

#include "spritereader.h"

#include <QLabel>
#include <QSharedPointer>
#include <QTimer>

#include <QElapsedTimer>

namespace mk2
{
class SpriteViewer : public QLabel
{
  Q_OBJECT

public:
  enum ScalingMode
  {
    NoScaling,
    WidthScaling,
    HeightScaling,
    StretchScaling,
    DynamicScaling,
  };
  Q_ENUM(ScalingMode)

  SpriteViewer(QWidget *parent = nullptr);
  ~SpriteViewer();

  QString get_file_name() const;

  QIODevice *get_device() const;

  SpriteViewer::ScalingMode get_scaling_mode() const;

  SpriteReader::ptr get_reader() const;

  void set_reader(SpriteReader::ptr reader);

  bool is_valid() const;

  bool is_running() const;

public slots:
  void set_file_name(QString file_name);

  void set_device(QIODevice *device);

  void set_play_once(bool on);

  void set_mirror(bool on);

  void set_scaling_mode(SpriteViewer::ScalingMode scaling_mode);

  void start();
  void restart();
  void stop();

signals:
  void file_name_changed(QString);

  void reader_changed();

  void started();
  void finished();

protected:
  void resizeEvent(QResizeEvent *event) final;

private:
  SpriteReader::ptr m_reader;
  SpriteFrame m_current_frame;
  SpriteViewer::ScalingMode m_scaling_mode;
  SpriteViewer::ScalingMode m_resolved_scaling_mode;
  Qt::TransformationMode m_transform;
  bool m_running;
  bool m_mirror;
  bool m_play_once;
  int m_frame_count;
  int m_frame_number;
  QElapsedTimer m_elapsed_timer;
  QTimer m_frame_timer;
  QTimer m_repaint_timer;

  void _p_resolve_scaling_mode();

private slots:
  void _p_paint_next_frame();
  void _p_paint_frame();
};
} // namespace mk2
