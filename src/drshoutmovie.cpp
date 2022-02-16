#include "drshoutmovie.h"

#include "aoapplication.h"
#include "file_functions.h"

DRShoutMovie::DRShoutMovie(QWidget *parent) : DRMovie(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{
  Q_ASSERT(ao_app);
  set_play_once(true);
  set_hide_on_done(true);
}

DRShoutMovie::~DRShoutMovie()
{}

void DRShoutMovie::play_interjection(QString p_character, QString p_shout)
{
  if (p_shout.toLower() == "custom")
  {
    p_shout.append("_bubble");
  }

  QString l_file_name =
      ao_app->find_asset_path({ao_app->get_character_path(p_character, p_shout)}, animated_extensions());

  if (l_file_name.isEmpty())
  {
    l_file_name = ao_app->find_theme_asset_path(p_shout, animated_extensions());
  }

  if (l_file_name.isEmpty())
  {
    qWarning() << "error: shout not found"
               << "character:" << p_character << "shout:" << p_shout;
  }

  set_file_name(l_file_name);
  set_play_once(true);
  start();
}
