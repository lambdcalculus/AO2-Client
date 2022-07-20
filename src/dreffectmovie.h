#pragma once

#include "drmovie.h"

class AOApplication;

class DREffectMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DREffectMovie(AOApplication *ao_app, QGraphicsItem *parent = nullptr);
  ~DREffectMovie();

  void play(QString p_file_name, QString p_character);
  void play(QString p_file_name);

private:
  AOApplication *ao_app;
};
