#pragma once

#include "drmovie.h"

class AOApplication;

class DRCharacterMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DRCharacterMovie(QWidget *parent = nullptr);
  ~DRCharacterMovie();

  void play(QString character, QString emote, QString prefix, bool p_use_placeholder, bool play_once);
  void play_pre(QString character, QString emote);
  void play_talk(QString character, QString emote);
  void play_idle(QString character, QString emote);

private:
  AOApplication *ao_app = nullptr;
};
