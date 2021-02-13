#include "aoshoutplayer.h"

#include "draudioengine.h"
#include "file_functions.h"

#include <QDebug>

AOShoutPlayer::AOShoutPlayer(AOApplication *p_ao_app, QObject *p_parent) : AOObject(p_ao_app, p_parent)
{}

void AOShoutPlayer::play(QString p_name, QString p_char)
{
  QString file;

  QString char_path = ao_app->get_character_path(p_char, p_name);
  QString theme_path = ao_app->find_theme_asset_path(p_name);

  qDebug() << char_path;
  qDebug() << theme_path;

  if (file_exists(char_path))
    file = char_path;
  else if (file_exists(theme_path))
    file = theme_path;

  if (file.isNull())
    return;
  DRAudioEngine::get_family(DRAudio::Family::FEffect)->play_stream(file);
}
