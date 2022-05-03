#pragma once

#include "drmovie.h"

class AOApplication;

class DRSceneMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DRSceneMovie(QWidget *parent = nullptr);
  ~DRSceneMovie();

  void set_background_image(QString p_background_name, QString p_image);

private:
  AOApplication *ao_app;
};
