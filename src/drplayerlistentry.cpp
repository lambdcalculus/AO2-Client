#include "drplayerlistentry.h"
#include "aoapplication.h"
#include "commondefs.h"
#include "file_functions.h"
#include "theme.h"

DrPlayerListEntry::DrPlayerListEntry(QWidget *p_parent, AOApplication *p_ao_app, int p_x, int p_y)
    : QWidget(p_parent)
{
    ao_app = p_ao_app;

    ui_user_image = new AOImageDisplay(this, ao_app);
    ui_showname = new AOLabel(this, ao_app);
    this->move(p_x, p_y);
    ui_user_image->move(0, 0);
    ui_user_image->resize(40, 40);
    ui_showname->move(45, 3);

    set_stylesheet(ui_showname, "[PLAYER NAME]", COURTROOM_STYLESHEETS_CSS, ao_app);

    set_stylesheet(ui_user_image, "[PLAYER ICON]", COURTROOM_STYLESHEETS_CSS, ao_app);


}


void DrPlayerListEntry::set_character(QString p_character)
{

  m_character = p_character;
  const QString l_icon_path = ao_app->get_character_path(m_character, "char_icon.png");

  const bool l_file_exist = file_exists(l_icon_path);
  if(l_file_exist)
  {
      ui_user_image->set_image(l_icon_path);

  }
  else
  {
      QString l_missing_char_image = ao_app->find_theme_asset_path("missing_char.png");
      if (!l_missing_char_image.isEmpty())
      {
          ui_user_image->set_theme_image("missing_char.png");
      }

  }



}

void DrPlayerListEntry::set_name(QString showname)
{
  m_showname = showname;
  ui_showname->setText(showname);



}
