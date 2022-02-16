#pragma once

#include "drmovie.h"

class AOApplication;

class DRStickerMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DRStickerMovie(QWidget *parent = nullptr);
  ~DRStickerMovie();

  void play(QString p_file_name, QString p_character);
  void play(QString p_file_name);

private:
  AOApplication *ao_app = nullptr;
};
