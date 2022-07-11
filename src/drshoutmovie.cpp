#include "drshoutmovie.h"

#include "aoapplication.h"
#include "file_functions.h"

#include <QDebug>

DRShoutMovie::DRShoutMovie(QWidget *parent)
    : DRMovie(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{
  Q_ASSERT(ao_app);
  set_play_once(true);
  set_hide_on_done(true);
}

DRShoutMovie::~DRShoutMovie()
{}

void DRShoutMovie::play_interjection(QString p_character, QString p_shout)
{
  set_file_name(ao_app->get_shout_sprite_path(p_character, p_shout));
  set_play_once(true);
  start();
}
