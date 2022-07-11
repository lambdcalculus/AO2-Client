#include "drstickermovie.h"

#include "aoapplication.h"
#include "file_functions.h"

DRStickerMovie::DRStickerMovie(QWidget *parent)
    : DRMovie(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{}

DRStickerMovie::~DRStickerMovie()
{}

void DRStickerMovie::play(QString p_file_name, QString p_character)
{
  set_file_name(ao_app->get_theme_sprite_path(p_file_name, p_character));
  start();
}

void DRStickerMovie::play(QString p_file_name)
{
  play(p_file_name, "");
}
