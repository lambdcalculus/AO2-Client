#include "drplayerlistentry.h"
#include "aoapplication.h"
#include "commondefs.h"
#include "file_functions.h"
#include "modules/theme/thememanager.h"
#include "theme.h"

DrPlayerListEntry::DrPlayerListEntry(QWidget *p_parent, AOApplication *p_ao_app, int p_x, int p_y)
    : QWidget(p_parent)
{

  int resize_height = (int)((float)50 * ThemeManager::get().getResize());

    ao_app = p_ao_app;
    m_entrywidth = p_parent->size().width();
    this->resize(m_entrywidth, resize_height);

    this->move(p_x, p_y);

    ui_showname = new AOLabel(this, ao_app);
    ui_showname->move(resize_height, 7);
    ui_showname->resize(m_entrywidth-resize_height, 16);
    set_stylesheet(ui_showname, "[PLAYER NAME]", COURTROOM_STYLESHEETS_CSS, ao_app);

    ui_user_image = new AOImageDisplay(this, ao_app);
    ui_user_image->move(5, 5);
    ui_user_image->resize((int)((float)40 * ThemeManager::get().getResize()), (int)((float)40 * ThemeManager::get().getResize()));


    pCharacterBorderDisplay = new AOImageDisplay(this, ao_app);
    pCharacterBorderDisplay->move(0, 0);
    pCharacterBorderDisplay->resize(resize_height, resize_height);

    const QString l_selected_texture = ao_app->find_theme_asset_path("char_border.png");

    if (file_exists(l_selected_texture)) pCharacterBorderDisplay->set_image(l_selected_texture);



    //set_stylesheet(ui_user_image, "[PLAYER ICON]", COURTROOM_STYLESHEETS_CSS, ao_app);


    //Prompt (For Blackouts / Look)
    m_prompt = new AOLabel(this, ao_app);
    m_prompt->move(5, 5);
    m_prompt->resize(m_entrywidth, resize_height);
    m_prompt->setWordWrap(true);
    set_stylesheet(m_prompt, "[PLAYER LIST PROMPT]", COURTROOM_STYLESHEETS_CSS, ao_app);

    ui_showname->hide();
    ui_user_image->hide();
    pCharacterBorderDisplay->hide();
    m_prompt->hide();
}


void DrPlayerListEntry::set_character(QString p_character)
{

  m_character = p_character;
  const QString l_icon_path = ao_app->get_character_path(m_character, "char_icon.png");

  const bool l_file_exist = file_exists(l_icon_path);
  if(l_file_exist)
  {
      ui_user_image->set_image(l_icon_path);

      const QString l_selected_texture = ao_app->get_character_path(p_character, "char_border.png");

      if (file_exists(l_selected_texture)) pCharacterBorderDisplay->set_image(l_selected_texture);

  }
  else
  {
      QString l_missing_char_image = ao_app->find_theme_asset_path("missing_char.png");
      if (!l_missing_char_image.isEmpty())
      {
          ui_user_image->set_theme_image("missing_char.png");
      }

  }
  ui_user_image->show();
  pCharacterBorderDisplay->show();
  m_prompt->hide();

}

void DrPlayerListEntry::set_name(QString showname)
{
  ui_showname->show();
  m_prompt->hide();
  m_showname = showname;
  ui_showname->setText(showname);

}

void DrPlayerListEntry::set_reason(QString p_reason)
{
  ui_showname->hide();
  ui_user_image->hide();
  pCharacterBorderDisplay->hide();
  m_prompt->show();

  m_prompt->setText(p_reason);
}
