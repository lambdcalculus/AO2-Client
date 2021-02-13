#include "aosfxplayer.h"

#include "draudioengine.h"
#include "file_functions.h"

#include <string.h>

#include <QDebug>

AOSfxPlayer::AOSfxPlayer(AOApplication *p_ao_app, QObject *p_parent) : AOObject(p_ao_app, p_parent)
{}

void AOSfxPlayer::play(QString p_name)
{
  const QString file = ao_app->find_asset_path({ao_app->get_sounds_path(p_name)}, audio_extensions());
  DRAudioEngine::get_family(DRAudio::Family::FEffect)->play_stream(file);
}
