#include "aosfxplayer.h"

#include "draudioengine.h"
#include "file_functions.h"

#include <string.h>

#include <QDebug>

AOSfxPlayer::AOSfxPlayer(AOApplication *p_ao_app, QObject *p_parent) : AOObject(p_ao_app, p_parent)
{}

void AOSfxPlayer::play(QString p_file)
{
  DRAudioEngine::get_family(DRAudio::Family::FEffect)->play_stream(p_file);
}

void AOSfxPlayer::play_effect(QString p_effect)
{
  play(ao_app->find_asset_path({ao_app->get_sounds_path(p_effect)}, audio_extensions()));
}

void AOSfxPlayer::play_character_effect(QString p_chr, QString p_effect)
{
  play_character_effect(QStringList{p_chr}, p_effect);
}

void AOSfxPlayer::play_character_effect(QStringList p_chr_list, QString p_effect)
{
  QStringList l_path_list;
  for (const QString &i_character : p_chr_list)
    l_path_list.append(ao_app->get_character_path(i_character, QString("sounds/%1").arg(p_effect)));

  const QString l_target_file = ao_app->find_asset_path(l_path_list, audio_extensions());
  if (l_target_file.isEmpty())
  {
    play_effect(p_effect);
    return;
  }
  play(l_target_file);
}

void AOSfxPlayer::stop_all()
{
  for (DRAudioStream::ptr &i_stream : DRAudioEngine::get_family(DRAudio::Family::FEffect)->get_stream_list())
    i_stream->stop();
}
