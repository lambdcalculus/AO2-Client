#include "drstickermovie.h"

#include "aoapplication.h"
#include "file_functions.h"

DRStickerMovie::DRStickerMovie(QWidget *parent) : DRMovie(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{}

DRStickerMovie::~DRStickerMovie()
{}

void DRStickerMovie::play(QString p_file_name, QString p_character)
{
  QString l_file_path;
  if (!p_character.isEmpty())
  {
    QString l_character_file_name = p_file_name;
    if (l_character_file_name == "custom")
    {
      l_character_file_name.append("_bubble");
    }

    QStringList l_path_list{
        ao_app->get_character_path(p_character, l_character_file_name),
        ao_app->get_character_path(p_character, "overlay/" + l_character_file_name),
    };
    l_file_path = ao_app->find_asset_path(l_path_list, animated_or_static_extensions());
  }

  if (l_file_path.isEmpty())
  {
    l_file_path = ao_app->find_theme_asset_path(p_file_name, animated_or_static_extensions());
    if (l_file_path.isEmpty())
    {
      l_file_path = ao_app->find_theme_asset_path("placeholder", animated_or_static_extensions());
    }
  }

  set_file_name(l_file_path);
  start();
}

void DRStickerMovie::play(QString p_file_name)
{
  play(p_file_name, "");
}
