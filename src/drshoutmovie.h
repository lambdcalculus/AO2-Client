#pragma once

#include "drmovie.h"

class AOApplication;

class DRShoutMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DRShoutMovie(AOApplication *ao_app, QGraphicsItem *parent = nullptr);
  ~DRShoutMovie();

  void play_interjection(QString p_char_name, QString p_interjection_name);

private:
  AOApplication *ao_app = nullptr;
};
