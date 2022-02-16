#pragma once

#include "drmovie.h"

class AOApplication;

class DRSceneMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DRSceneMovie(QWidget *parent = nullptr);
  ~DRSceneMovie();

  void set_image(QString p_image);

private:
  AOApplication *ao_app;
};
