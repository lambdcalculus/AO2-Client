#pragma once

#include "drmovie.h"

class AOApplication;

class DRSplashMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DRSplashMovie(AOApplication *ao_app, QGraphicsItem *parent = nullptr);
  ~DRSplashMovie();

  void play(QString p_file_name, QString p_character);
  void play(QString p_file_name);

private:
  AOApplication *ao_app;
};
