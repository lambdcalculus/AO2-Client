#include "courtroom.h"

#include "aoapplication.h"
#include "aobutton.h"
#include "aoconfig.h"
#include "aoemotebutton.h"
#include "aoimagedisplay.h"
#include "commondefs.h"
#include "drcharactermovie.h"
#include "drgraphicscene.h"
#include "theme.h"
#include "drtheme.h"
#include "modules/theme/thememanager.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QScreen>
#include <QtMath>



void Courtroom::construct_emotes()
{
  ui_emotes = new QWidget(this);

  ui_emote_left = new AOButton(this, ao_app);
  ui_emote_right = new AOButton(this, ao_app);

  ui_emote_preview = new DRGraphicsView(nullptr);
  ui_emote_preview->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::BypassGraphicsProxyWidget);
  ui_emote_preview->setAttribute(Qt::WA_TransparentForMouseEvents);

  {
    auto *l_scene = ui_emote_preview->scene();

    ui_emote_preview_background = new DRThemeMovie(ao_app);
    l_scene->addItem(ui_emote_preview_background);

    ui_emote_preview_character = new DRCharacterMovie(ao_app);
    l_scene->addItem(ui_emote_preview_character);
  }

  ui_emote_dropdown = new QComboBox(this);
  ui_pos_dropdown = new QComboBox(this);
  ui_pos_dropdown->addItem("Default");
  ui_pos_dropdown->addItem("Witness", "wit");
  ui_pos_dropdown->addItem("Defense", "def");
  ui_pos_dropdown->addItem("Prosecutor", "pro");
  ui_pos_dropdown->addItem("Judge", "jud");
  ui_pos_dropdown->addItem("Defense Assistant", "hld");
  ui_pos_dropdown->addItem("Prosecutor Assistant", "hlp");

  construct_emote_page_layout();
}

void Courtroom::construct_emote_page_layout()
{
  // delete previous buttons
  while (!ui_emote_list.isEmpty())
    delete ui_emote_list.takeLast();

  // resize and move
  set_size_and_pos(ui_emotes, "emotes", COURTROOM_DESIGN_INI, ao_app);

  QPoint f_spacing = ao_app->current_theme->get_widget_settings_spacing("emotes", "courtroom", "emote_button_spacing");

  int buttonResize = (int)((float)40 * ThemeManager::get().getResize());
  const int button_width = buttonResize;
  int x_spacing = f_spacing.x();
  int x_mod_count = 0;

  const int button_height = buttonResize;
  int y_spacing = f_spacing.y();
  int y_mod_count = 0;

  emote_columns = ((ui_emotes->width() - button_width) / (x_spacing + button_width)) + 1;
  emote_rows = ((ui_emotes->height() - button_height) / (y_spacing + button_height)) + 1;

  m_page_max_emote_count = qMax(1, emote_columns * emote_rows);
  for (int n = 0; n < m_page_max_emote_count; ++n)
  {
    int x_pos = (button_width + x_spacing) * x_mod_count;
    int y_pos = (button_height + y_spacing) * y_mod_count;

    AOEmoteButton *f_emote = new AOEmoteButton(ui_emotes, ao_app, x_pos, y_pos);

    ui_emote_list.append(f_emote);

    f_emote->set_emote_number(n);

    connect(f_emote, SIGNAL(emote_clicked(int)), this, SLOT(on_emote_clicked(int)));
    connect(f_emote, SIGNAL(tooltip_requested(int, QPoint)), this, SLOT(show_emote_tooltip(int, QPoint)));
    connect(f_emote, SIGNAL(mouse_left(int)), this, SLOT(hide_emote_tooltip(int)));

    ++x_mod_count;

    if (x_mod_count == emote_columns)
    {
      ++y_mod_count;
      x_mod_count = 0;
    }
  }

  refresh_emote_page(true);
}

void Courtroom::reset_emote_page()
{
  m_emote_id = 0;
  m_current_emote_page = 0;
  if (ui_emote_dropdown->count())
    ui_emote_dropdown->setCurrentIndex(m_emote_id);
  refresh_emote_page(true);
}

void Courtroom::refresh_emote_page(const bool p_scroll_to_current_emote)
{
  ui_emote_left->hide();
  ui_emote_right->hide();

  for (AOEmoteButton *i_button : qAsConst(ui_emote_list))
    i_button->hide();
  hide_emote_tooltip(m_emote_preview_id);

  if (is_spectating())
    return;

  const int l_emote_count = m_emote_list.length();
  const int l_page_count = qFloor(l_emote_count / m_page_max_emote_count) + bool(l_emote_count % m_page_max_emote_count);

  if (p_scroll_to_current_emote)
    m_current_emote_page = m_emote_id / m_page_max_emote_count;
  m_current_emote_page = qBound(0, m_current_emote_page, l_page_count - 1);

  const int l_current_page_emote_count = qBound(0, l_emote_count - m_current_emote_page * m_page_max_emote_count, m_page_max_emote_count);

  if (m_current_emote_page + 1 < l_page_count)
    ui_emote_right->show();

  if (m_current_emote_page > 0)
    ui_emote_left->show();

  for (int i = 0; i < l_current_page_emote_count; ++i)
  {
    const int l_real_i = i + m_current_emote_page * m_page_max_emote_count;
    AOEmoteButton *l_button = ui_emote_list.at(i);
    l_button->set_image(get_emote(l_real_i), l_real_i == m_emote_id);
    l_button->show();
  }
}

void Courtroom::fill_emote_dropdown()
{
  QSignalBlocker l_blocker(ui_emote_dropdown);
  ui_emote_dropdown->clear();

  QStringList l_emote_list;
  for (const DREmote &i_emote : qAsConst(m_emote_list))
    l_emote_list.append(i_emote.comment);
  ui_emote_dropdown->addItems(l_emote_list);
}

DREmote Courtroom::get_emote(const int p_emote_id)
{
  if (p_emote_id < 0 || p_emote_id >= m_emote_list.length())
    return DREmote();
  return m_emote_list.at(p_emote_id);
}

DREmote Courtroom::get_current_emote()
{
  return get_emote(m_emote_id);
}

void Courtroom::select_emote(int p_id)
{
  const int l_min = m_current_emote_page * m_page_max_emote_count;
  const int l_max = (m_page_max_emote_count - 1) + m_current_emote_page * m_page_max_emote_count;

  const DREmote &l_prev_emote = get_emote(m_emote_id);
  if (m_emote_id >= l_min && m_emote_id <= l_max)
  {
    AOEmoteButton *l_prev_button = ui_emote_list.at(m_emote_id % m_page_max_emote_count);
    l_prev_button->set_image(l_prev_emote, false);
    l_prev_button->repaint();
  }

  const int l_prev_emote_id = m_emote_id;
  m_emote_id = p_id;
  const DREmote &l_emote = get_emote(m_emote_id);

  if (m_emote_id >= l_min && m_emote_id <= l_max)
  {
    AOEmoteButton *l_new_button = ui_emote_list.at(m_emote_id % m_page_max_emote_count);
    l_new_button->set_image(l_emote, true);
    l_new_button->repaint();
  }

  const int emote_mod = l_emote.modifier;
  if (l_prev_emote_id == m_emote_id) // toggle
    ui_pre->setChecked(!ui_pre->isChecked());
  else
    ui_pre->setChecked(emote_mod == 1 || ao_config->always_pre_enabled());

  if (!ao_config->sticky_sfx_enabled())
    select_default_sfx();

  ui_emote_dropdown->setCurrentIndex(m_emote_id);

  ui_ic_chat_message_field->setFocus();
}

void Courtroom::on_emote_clicked(int p_id)
{
  select_emote(p_id + m_page_max_emote_count * m_current_emote_page);
}

void Courtroom::show_emote_tooltip(int p_id, QPoint p_global_pos)
{
  if (!ao_config->emote_preview_enabled())
    return;

  if (m_emote_preview_id != -1 || m_emote_preview_id == p_id)
    return;
  m_emote_preview_id = p_id;
  const int l_real_id = p_id + m_page_max_emote_count * m_current_emote_page;
  const DREmote &l_emote = m_emote_list.at(l_real_id);
  ui_emote_preview_character->set_mirrored(ui_flip->isChecked());
  ui_emote_preview_character->play_idle(l_emote.character, l_emote.dialog);

  QScreen *screen = QApplication::screenAt(p_global_pos);
  if (screen == nullptr)
  {
    return;
  }
  QRect l_screen_geometry = screen->geometry();

  // position below cursor
  const int l_vertical_spacing = 8;
  QPoint l_final_global_pos(p_global_pos.x(), p_global_pos.y() + l_vertical_spacing);

  if (l_screen_geometry.width() < ui_emote_preview->width() + l_final_global_pos.x())
  {
    l_final_global_pos.setX(p_global_pos.x() - ui_emote_preview->width());
  }

  if (l_screen_geometry.height() < ui_emote_preview->height() + l_final_global_pos.y())
  {
    l_final_global_pos.setY(p_global_pos.y() - ui_emote_preview->height() - l_vertical_spacing);
  }

  ui_emote_preview->move(l_final_global_pos);
  ui_emote_preview->show();
}

void Courtroom::hide_emote_tooltip(int p_id)
{
  if (m_emote_preview_id == -1 || m_emote_preview_id != p_id)
    return;
  m_emote_preview_id = -1;
  ui_emote_preview->hide();
  ui_emote_preview_character->set_file_name(nullptr);
  ui_emote_preview_character->stop();
}

void Courtroom::on_emote_preview_toggled(bool p_enabled)
{
  if (!p_enabled)
    hide_emote_tooltip(m_emote_preview_id);
}

void Courtroom::on_emote_left_clicked()
{
  --m_current_emote_page;

  refresh_emote_page();

  ui_ic_chat_message_field->setFocus();
}

void Courtroom::on_emote_right_clicked()
{
  ++m_current_emote_page;

  refresh_emote_page();

  ui_ic_chat_message_field->setFocus();
}

void Courtroom::on_emote_dropdown_changed(int p_index)
{
  select_emote(p_index);
}
