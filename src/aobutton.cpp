#include "aobutton.h"

#include "aoapplication.h"
#include "debug_functions.h"
#include "file_functions.h"

#include <QDebug>

AOButton::AOButton(QWidget *parent, AOApplication *p_ao_app) : QPushButton(parent)
{
  ao_app = p_ao_app;
}

QString AOButton::get_image()
{
  return image_path;
}

void AOButton::set_image(QString p_image)
{
  image_path = ao_app->find_theme_asset_path(p_image);
  // Get the path of the found image without the extension
  QString image_name = p_image.left(p_image.lastIndexOf(QChar('.')));
  QString hover_image_path = ao_app->find_theme_asset_path(image_name + "_hover.png");

  if (file_exists(image_path))
  {
    if (file_exists(hover_image_path))
      this->setStyleSheet("QPushButton {border-image:url(\"" + image_path +
                          "\");}"
                          "QPushButton:hover {border-image:url(\"" +
                          hover_image_path + "\");}");
    else
      this->setStyleSheet("border-image:url(\"" + image_path + "\")");
  }
  else
  {
    image_path = "";
    this->setStyleSheet("border-image:url(\"" + image_path + "\")");
  }
}
