#include "courtroom.h"

#include "aoapplication.h"
#include "aobutton.h"
#include "aoconfig.h"
#include "aoemotebutton.h"
#include "commondefs.h"
#include "theme.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QtMath>

void Courtroom::construct_emotes()
{
  ui_emotes = new QWidget(this);

  ui_emote_left = new AOButton(this, ao_app);
  ui_emote_right = new AOButton(this, ao_app);

  ui_emote_dropdown = new QComboBox(this);
  ui_pos_dropdown = new QComboBox(this);
  ui_pos_dropdown->addItem("wit", "wit");
  ui_pos_dropdown->addItem("def", "def");
  ui_pos_dropdown->addItem("pro", "pro");
  ui_pos_dropdown->addItem("jud", "jud");
  ui_pos_dropdown->addItem("hld", "hld");
  ui_pos_dropdown->addItem("hlp", "hlp");

  construct_emote_page_layout();
}

void Courtroom::construct_emote_page_layout()
{
  // delete previous buttons
  while (!ui_emote_list.isEmpty())
    delete ui_emote_list.takeLast();

  // resize and move
  set_size_and_pos(ui_emotes, "emotes", COURTROOM_DESIGN_INI, ao_app);

  QPoint f_spacing = ao_app->get_button_spacing("emote_button_spacing", COURTROOM_DESIGN_INI);

  const int button_width = 40;
  int x_spacing = f_spacing.x();
  int x_mod_count = 0;

  const int button_height = 40;
  int y_spacing = f_spacing.y();
  int y_mod_count = 0;

  emote_columns = ((ui_emotes->width() - button_width) / (x_spacing + button_width)) + 1;
  emote_rows = ((ui_emotes->height() - button_height) / (y_spacing + button_height)) + 1;

  m_page_max_emote_count = emote_columns * emote_rows;

  for (int n = 0; n < m_page_max_emote_count; ++n)
  {
    int x_pos = (button_width + x_spacing) * x_mod_count;
    int y_pos = (button_height + y_spacing) * y_mod_count;

    AOEmoteButton *f_emote = new AOEmoteButton(ui_emotes, ao_app, x_pos, y_pos);

    ui_emote_list.append(f_emote);

    f_emote->set_emote_number(n);

    connect(f_emote, SIGNAL(emote_clicked(int)), this, SLOT(on_emote_clicked(int)));

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

  if (is_spectating())
    return;

  const int l_emote_count = m_emote_list.length();
  for (AOEmoteButton *i_button : qAsConst(ui_emote_list))
    i_button->hide();

  const int l_page_count =
      qFloor(l_emote_count / m_page_max_emote_count) + bool(l_emote_count % m_page_max_emote_count);

  if (p_scroll_to_current_emote)
    m_current_emote_page = m_emote_id / m_page_max_emote_count;
  m_current_emote_page = qBound(0, m_current_emote_page, l_page_count - 1);

  const int l_current_page_emote_count =
      qBound(0, l_emote_count - m_current_emote_page * m_page_max_emote_count, m_page_max_emote_count);

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
    ui_emote_list.at(m_emote_id % m_page_max_emote_count)->set_image(l_prev_emote, false);

  const int l_prev_emote_id = m_emote_id;
  m_emote_id = p_id;
  const DREmote &l_emote = get_emote(m_emote_id);

  if (m_emote_id >= l_min && m_emote_id <= l_max)
    ui_emote_list.at(m_emote_id % m_page_max_emote_count)->set_image(l_emote, true);

  const int emote_mod = l_emote.modifier;
  if (l_prev_emote_id == m_emote_id) // toggle
    ui_pre->setChecked(!ui_pre->isChecked());
  else
    ui_pre->setChecked(emote_mod == 1 || ao_config->always_pre_enabled());

  select_default_sfx();

  ui_emote_dropdown->setCurrentIndex(m_emote_id);

  ui_ic_chat_message->setFocus();
}

void Courtroom::on_emote_clicked(int p_id)
{
  select_emote(p_id + m_page_max_emote_count * m_current_emote_page);
}

void Courtroom::on_emote_left_clicked()
{
  --m_current_emote_page;

  refresh_emote_page();

  ui_ic_chat_message->setFocus();
}

void Courtroom::on_emote_right_clicked()
{
  ++m_current_emote_page;

  refresh_emote_page();

  ui_ic_chat_message->setFocus();
}

void Courtroom::on_emote_dropdown_changed(int p_index)
{
  select_emote(p_index);
}
