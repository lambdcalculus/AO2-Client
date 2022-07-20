#pragma once

#include "drmovie.h"

class AOApplication;

class DRThemeMovie : public DRMovie
{
  Q_OBJECT

public:
  DRThemeMovie(AOApplication *app, QGraphicsItem *parent = nullptr);
  ~DRThemeMovie();

public slots:
  void set_theme_image(QString p_image);

private:
  AOApplication *ao_app;
};
