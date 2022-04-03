#include "aoimagedisplay.h"

#include "aoapplication.h"
#include "aopixmap.h"
#include "file_functions.h"

/*!
 * @class AOImageDisplay
 * @brief Represents a static theme-dependent image.
 */
AOImageDisplay::AOImageDisplay(QWidget *parent, AOApplication *p_ao_app) : QLabel(parent)
{
  ao_app = p_ao_app;
}

QString AOImageDisplay::get_image()
{
  return m_image;
}

void AOImageDisplay::set_image(QString p_image)
{
  m_image = p_image;
  AOPixmap l_pixmap(p_image);
  setPixmap(l_pixmap.scale(size()));
}

void AOImageDisplay::set_theme_image(QString p_image)
{
  set_image(ao_app->find_theme_asset_path(p_image));
}

void AOImageDisplay::set_chatbox_image(QString p_image)
{
  QString l_target_path = ao_app->find_asset_path(ao_app->get_base_path() + "misc/" + p_image + ".png");
  if (!file_exists(l_target_path))
  {
    l_target_path = ao_app->find_theme_asset_path("chatmed.png");
  }
  set_image(l_target_path);
}
