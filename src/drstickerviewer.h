#pragma once

#include "mk2/spriteviewer.h"

class AOApplication;

class DRStickerViewer : public mk2::SpriteViewer
{
  Q_OBJECT

public:
  explicit DRStickerViewer(AOApplication *ao_app, QWidget *parent = nullptr);
  ~DRStickerViewer();

  void set_hide_when_done(bool on);

  void set_theme_image(QString p_file_name, QString p_character);

  void set_theme_image(QString p_file_name);

private:
  AOApplication *ao_app = nullptr;

  bool m_hide_when_done;

private slots:
  void maybe_hide();
};
