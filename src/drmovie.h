#pragma once

#include "mk2/spriteviewer.h"

class DRMovie : public mk2::SpriteViewer
{
  Q_OBJECT

public:
  DRMovie(QWidget *parent = nullptr);
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
  void _p_update_visibility();
};
