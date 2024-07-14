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

#include "modules/theme/droanimation.h"
#include "spriteplayer.h"
#include "spritereader.h"

#include <QGraphicsObject>
#include <QObject>
#include <QPainter>

namespace mk2
{
class GraphicsSpriteItem : public QGraphicsObject
{
  Q_OBJECT

  Q_PROPERTY(QSizeF size READ get_size WRITE set_size NOTIFY size_changed)

public:
  using ScalingMode = SpritePlayer::ScalingMode;

  GraphicsSpriteItem(QGraphicsItem *parent = nullptr);
  ~GraphicsSpriteItem();

  SpritePlayer::ScalingMode get_scaling_mode() const;

  QSizeF get_size() const;

  QString get_file_name() const;

  QIODevice *get_device() const;

  SpriteReader::ptr get_reader() const;

  SpritePlayer *get_player() const;

  bool is_valid() const;

  bool is_running() const;


  QRectF boundingRect() const final;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) final;

public slots:
  void set_scaling_mode(SpritePlayer::ScalingMode scaling_mode);

  void set_center_mode(bool t_center);

  void set_composition_mode(QPainter::CompositionMode t_composition);

  void set_size(QSizeF size);

  void set_play_once(bool enabled);

  void set_mirror(bool enabled);

  void set_file_name(QString file_name);

  void set_device(QIODevice *device);

  void set_reader(SpriteReader::ptr reader);

  void start();
  void restart();
  void setVerticalOffset(int t_offset);

  void ResetAnimation()
  {
    setScale(1);
    setRotation(0);
    setOpacity(1);
    setTransformOffset(0, 0);
  }

  void setTransformOffset(int t_x, int t_y)
  {
    if(t_x != -11037) m_TransformX = t_x;
    if(t_y != -11037) m_TransformY = t_y;
    update();
  };

  void setTransformX(int t_x)
  {
    if(t_x == -11037) return;
    m_TransformX = t_x;
    update();
  };

  void setTransformY(int t_y)
  {
    if(t_y == -11037) return;
    m_TransformY = t_y;
    update();
  };

  void setBackgroundScaling(double t_offset);

  void setCurrentAnimation(DROAnimation* t_animation);
  void stop();

signals:
  void size_changed(QSizeF);

  void file_name_changed(QString);

  void reader_changed();

  void started();
  void finished();

private:
  int m_TransformX = 0;
  int m_TransformY = 0;

  QScopedPointer<SpritePlayer> m_player;
  QPainter::CompositionMode mCompoMode = QPainter::CompositionMode_SourceOver;
  int mVerticalVPOffset = 0;
  bool mCenterSprite = true;
  DROAnimation* mWidgetAnimation = nullptr;

private slots:
  void notify_size();

  void notify_update();
};
} // namespace mk2
