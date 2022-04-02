#pragma once

#include "drmovie.h"

class AOApplication;

class DRCharacterMovie : public DRMovie
{
  Q_OBJECT

public:
  explicit DRCharacterMovie(QWidget *parent = nullptr);
  ~DRCharacterMovie();

  bool is_playing_empty(QString p_character, QString p_emote);
  void play(QString character, QString emote, QString prefix, bool play_once, bool p_hide_if_empty);
  void play_pre(QString character, QString emote);
  void play_talk(QString character, QString emote);
  void play_idle(QString character, QString emote);

private:
  AOApplication *ao_app = nullptr;
};
