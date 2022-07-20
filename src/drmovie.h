#pragma once

#include "mk2/graphicsspriteitem.h"

class DRMovie : public mk2::GraphicsSpriteItem
{
  Q_OBJECT

public:
  DRMovie(QGraphicsItem *parent = nullptr);
  ~DRMovie();

  QString file_name();
  void set_hide_on_done(bool on);
  void set_mirrored(bool on);

signals:
  void done();

private:
  bool m_hide_when_done;
  bool m_mirrored;

private slots:
  void update_visibility();
};
