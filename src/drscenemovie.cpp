#include "commondefs.h"
#include "drscenemovie.h"

#include "aoapplication.h"
#include "file_functions.h"

DRSceneMovie::DRSceneMovie(QWidget *parent) : DRMovie(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{
  Q_ASSERT(ao_app);
  set_scale_to_height(true);
}

DRSceneMovie::~DRSceneMovie()
{}

void DRSceneMovie::set_image(QString p_image)
{
  const QString l_file_path =
      ao_app->find_asset_path(ao_app->get_current_background_path() + "/" + p_image, animated_or_static_extensions());

  if (file_name() == l_file_path)
    return;

  set_file_name(l_file_path);
  start();
}
