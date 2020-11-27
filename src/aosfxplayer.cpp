#include "aosfxplayer.h"
#include "file_functions.h"

#include <string.h>

#include <QDebug>

AOSfxPlayer::AOSfxPlayer(QObject *p_parent, AOApplication *p_ao_app)
    : AOAbstractPlayer(p_parent, p_ao_app)
{
}

void AOSfxPlayer::play(QString p_name)
{
  QString f_root = ao_app->get_sounds_path(p_name);
  QString f_file = ao_app->find_asset_path({f_root}, audio_extensions());

  try
  {
    AOBassHandle *handle = new AOBassHandle(f_file, true, this);
    connect(this, &AOSfxPlayer::new_volume, handle, &AOBassHandle::set_volume);
    connect(this, &AOSfxPlayer::stopping, handle, &AOBassHandle::stop);
    handle->set_volume(get_volume());
    handle->play();
  }
  catch (const std::exception &e_exception)
  {
    qDebug() << e_exception.what();
  }
}

void AOSfxPlayer::stop()
{
  Q_EMIT stopping();
}
