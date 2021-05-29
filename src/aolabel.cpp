#include "aolabel.h"

#include "aoapplication.h"

AOLabel::AOLabel(QWidget *parent, AOApplication *p_ao_app) : QLabel(parent)
{
  ao_app = p_ao_app;
}

void AOLabel::set_image(QString p_image)
{
  QString f_path = ao_app->find_theme_asset_path(p_image);
  setStyleSheet("border-image:url(\"" + f_path + "\")");
}
