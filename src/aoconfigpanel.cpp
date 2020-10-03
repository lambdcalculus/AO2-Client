#include "aoconfigpanel.h"
// qt
#include <QDebug>
#include <QDir>

AOConfigPanel::AOConfigPanel(QWidget *p_parent)
    : QWidget(p_parent), m_config(new AOConfig(this))
{
  setWindowTitle(tr("Config"));
  setWindowFlag(Qt::WindowMinMaxButtonsHint, false);

  AOGuiLoader loader;
  loader.load_from_file(":res/ui/config_panel.ui", this);

  // tab
  setFocusProxy(AO_GUI_WIDGET(QTabWidget, "tab_widget"));

  // save
  w_save = AO_GUI_WIDGET(QPushButton, "save");

  // general
  w_username = AO_GUI_WIDGET(QLineEdit, "username");
  w_callwords = AO_GUI_WIDGET(QLineEdit, "callwords");
  w_server_alerts = AO_GUI_WIDGET(QCheckBox, "server_alerts");

  // game
  w_theme = AO_GUI_WIDGET(QComboBox, "theme");
  w_reload_theme = AO_GUI_WIDGET(QPushButton, "theme_reload");
  w_gamemode = AO_GUI_WIDGET(QComboBox, "gamemode");
  w_manual_gamemode = AO_GUI_WIDGET(QCheckBox, "manual_gamemode");
  w_timeofday = AO_GUI_WIDGET(QComboBox, "timeofday");
  w_manual_timeofday = AO_GUI_WIDGET(QCheckBox, "manual_timeofday");
  w_always_pre = AO_GUI_WIDGET(QCheckBox, "always_pre");
  w_chat_tick_interval = AO_GUI_WIDGET(QSpinBox, "chat_tick_interval");

  // IC Chatlog
  w_log_max_lines = AO_GUI_WIDGET(QSpinBox, "log_length");
  w_log_uses_newline = AO_GUI_WIDGET(QCheckBox, "log_newline");
  w_log_orientation_top_down =
      AO_GUI_WIDGET(QRadioButton, "log_orientation_top_down");
  w_log_orientation_bottom_up =
      AO_GUI_WIDGET(QRadioButton, "log_orientation_bottom_up");
  w_log_music = AO_GUI_WIDGET(QCheckBox, "log_music");
  w_log_is_recording = AO_GUI_WIDGET(QCheckBox, "log_recording");

  // audio
  w_effects = AO_GUI_WIDGET(QSlider, "effects");
  w_effects_value = AO_GUI_WIDGET(QLabel, "effects_value");
  w_system = AO_GUI_WIDGET(QSlider, "system");
  w_system_value = AO_GUI_WIDGET(QLabel, "system_value");
  w_music = AO_GUI_WIDGET(QSlider, "music");
  w_music_value = AO_GUI_WIDGET(QLabel, "music_value");
  w_blips = AO_GUI_WIDGET(QSlider, "blips");
  w_blips_value = AO_GUI_WIDGET(QLabel, "blips_value");
  w_blip_rate = AO_GUI_WIDGET(QSpinBox, "blip_rate");
  w_blank_blips = AO_GUI_WIDGET(QCheckBox, "blank_blips");

  // themes
  refresh_theme_list();
  refresh_gamemode_list();
  refresh_timeofday_list();

  // input
  connect(m_config, SIGNAL(username_changed(QString)), w_username,
          SLOT(setText(QString)));
  connect(m_config, SIGNAL(callwords_changed(QString)), w_callwords,
          SLOT(setText(QString)));
  connect(m_config, SIGNAL(server_alerts_changed(bool)), w_server_alerts,
          SLOT(setChecked(bool)));
  connect(m_config, SIGNAL(theme_changed(QString)), w_theme,
          SLOT(setCurrentText(QString)));
  connect(m_config, SIGNAL(gamemode_changed(QString)), w_gamemode,
          SLOT(setCurrentText(QString)));
  connect(m_config, SIGNAL(manual_gamemode_changed(bool)), w_manual_gamemode,
          SLOT(setChecked(bool)));
  connect(m_config, SIGNAL(timeofday_changed(QString)), w_timeofday,
          SLOT(setCurrentText(QString)));
  connect(m_config, SIGNAL(manual_timeofday_changed(bool)), w_manual_timeofday,
          SLOT(setChecked(bool)));
  connect(m_config, SIGNAL(always_pre_changed(bool)), w_always_pre,
          SLOT(setChecked(bool)));
  connect(m_config, SIGNAL(chat_tick_interval_changed(int)),
          w_chat_tick_interval, SLOT(setValue(int)));
  connect(m_config, SIGNAL(log_max_lines_changed(int)), w_log_max_lines,
          SLOT(setValue(int)));
  connect(m_config, SIGNAL(log_is_topdown_changed(bool)), this,
          SLOT(on_log_is_topdown_changed(bool)));
  connect(m_config, SIGNAL(log_uses_newline_changed(bool)), w_log_uses_newline,
          SLOT(setChecked(bool)));
  connect(m_config, SIGNAL(log_music_changed(bool)), w_log_music,
          SLOT(setChecked(bool)));
  connect(m_config, SIGNAL(log_is_recording_changed(bool)), w_log_is_recording,
          SLOT(setChecked(bool)));
  connect(m_config, SIGNAL(effects_volume_changed(int)), w_effects,
          SLOT(setValue(int)));
  connect(m_config, SIGNAL(system_volume_changed(int)), w_system,
          SLOT(setValue(int)));
  connect(m_config, SIGNAL(music_volume_changed(int)), w_music,
          SLOT(setValue(int)));
  connect(m_config, SIGNAL(blips_volume_changed(int)), w_blips,
          SLOT(setValue(int)));
  connect(m_config, SIGNAL(blip_rate_changed(int)), w_blip_rate,
          SLOT(setValue(int)));
  connect(m_config, SIGNAL(blank_blips_changed(bool)), w_blank_blips,
          SLOT(setChecked(bool)));

  // output
  connect(w_save, SIGNAL(clicked()), m_config, SLOT(save_file()));
  connect(w_username, SIGNAL(textEdited(QString)), m_config,
          SLOT(set_username(QString)));
  connect(w_callwords, SIGNAL(textEdited(QString)), m_config,
          SLOT(set_callwords(QString)));
  connect(w_server_alerts, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_server_alerts(int)));
  connect(w_theme, SIGNAL(currentIndexChanged(QString)), m_config,
          SLOT(set_theme(QString)));
  connect(w_reload_theme, SIGNAL(clicked()), this,
          SLOT(on_reload_theme_clicked()));
  connect(w_gamemode, SIGNAL(currentIndexChanged(QString)), this,
          SLOT(on_gamemode_index_changed(QString)));
  connect(w_manual_gamemode, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_manual_gamemode(int)));
  connect(w_timeofday, SIGNAL(currentIndexChanged(QString)), this,
          SLOT(on_timeofday_index_changed(QString)));
  connect(w_manual_timeofday, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_manual_timeofday(int)));
  connect(w_always_pre, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_always_pre(int)));
  connect(w_chat_tick_interval, SIGNAL(valueChanged(int)), m_config,
          SLOT(set_chat_tick_interval(int)));
  connect(w_log_max_lines, SIGNAL(valueChanged(int)), m_config,
          SLOT(set_log_max_lines(int)));
  connect(w_log_orientation_top_down, SIGNAL(toggled(bool)), m_config,
          SLOT(set_log_is_topdown(bool)));
  connect(w_log_uses_newline, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_log_uses_newline(int)));
  connect(w_log_music, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_log_music(int)));
  connect(w_log_is_recording, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_log_is_recording(int)));
  connect(w_effects, SIGNAL(valueChanged(int)), m_config,
          SLOT(set_effects_volume(int)));
  connect(w_effects, SIGNAL(valueChanged(int)), this,
          SLOT(on_effects_value_changed(int)));
  connect(w_system, SIGNAL(valueChanged(int)), m_config,
          SLOT(set_system_volume(int)));
  connect(w_system, SIGNAL(valueChanged(int)), this,
          SLOT(on_system_value_changed(int)));
  connect(w_music, SIGNAL(valueChanged(int)), m_config,
          SLOT(set_music_volume(int)));
  connect(w_music, SIGNAL(valueChanged(int)), this,
          SLOT(on_music_value_changed(int)));
  connect(w_blips, SIGNAL(valueChanged(int)), m_config,
          SLOT(set_blips_volume(int)));
  connect(w_blips, SIGNAL(valueChanged(int)), this,
          SLOT(on_blips_value_changed(int)));
  connect(w_blip_rate, SIGNAL(valueChanged(int)), m_config,
          SLOT(set_blip_rate(int)));
  connect(w_blank_blips, SIGNAL(stateChanged(int)), m_config,
          SLOT(set_blank_blips(int)));

  // set values
  w_username->setText(m_config->username());
  w_callwords->setText(m_config->callwords());
  w_server_alerts->setChecked(m_config->server_alerts_enabled());
  w_theme->setCurrentText(m_config->theme());
  w_gamemode->setCurrentText(m_config->gamemode());
  w_manual_gamemode->setChecked(m_config->manual_gamemode_enabled());
  w_timeofday->setCurrentText(m_config->timeofday());
  w_manual_timeofday->setChecked(m_config->manual_timeofday_enabled());
  w_always_pre->setChecked(m_config->always_pre_enabled());
  w_chat_tick_interval->setValue(m_config->chat_tick_interval());
  w_log_max_lines->setValue(m_config->log_max_lines());
  if (m_config->log_is_topdown_enabled())
    w_log_orientation_top_down->setChecked(true);
  else
    w_log_orientation_bottom_up->setChecked(true);
  w_log_uses_newline->setChecked(m_config->log_uses_newline_enabled());
  w_log_music->setChecked(m_config->log_music_enabled());
  w_log_is_recording->setChecked(m_config->log_is_recording_enabled());
  w_effects->setValue(m_config->effects_volume());
  w_system->setValue(m_config->system_volume());
  w_music->setValue(m_config->music_volume());
  w_blips->setValue(m_config->blips_volume());
  w_blip_rate->setValue(m_config->blip_rate());
  w_blank_blips->setChecked(m_config->blank_blips_enabled());

  // Widget enabling connections
  w_gamemode->setEnabled(m_config->manual_gamemode_enabled());
  w_timeofday->setEnabled(m_config->manual_timeofday_enabled());
  // The manual gamemode checkbox enables browsing the gamemode combox
  // similarly with time of day
  connect(m_config, SIGNAL(manual_gamemode_changed(bool)), w_gamemode,
          SLOT(setEnabled(bool)));
  connect(m_config, SIGNAL(manual_timeofday_changed(bool)), w_timeofday,
          SLOT(setEnabled(bool)));
}

void AOConfigPanel::showEvent(QShowEvent *event)
{
  QWidget::showEvent(event);

  if (isVisible())
  {
    // refresh theme, gamemode and time of day comboboxes
    refresh_theme_list();
    refresh_gamemode_list();
    refresh_timeofday_list();
  }
}

void AOConfigPanel::refresh_theme_list()
{
  const QString p_prev_text = w_theme->currentText();

  // block signals
  w_theme->blockSignals(true);
  w_theme->clear();

  // themes
  for (QString i_folder :
       QDir(QDir::currentPath() + "/base/themes").entryList(QDir::Dirs))
  {
    if (i_folder == "." || i_folder == "..")
      continue;
    w_theme->addItem(i_folder);
  }

  // restore previous selection
  w_theme->setCurrentText(p_prev_text);

  // unblock
  w_theme->blockSignals(false);
}

void AOConfigPanel::refresh_gamemode_list()
{
  const QString p_prev_text = w_gamemode->currentText();

  // block signals
  w_gamemode->blockSignals(true);
  w_gamemode->clear();

  // add empty entry indicating no gamemode chosen
  w_gamemode->addItem("<default>");
  // gamemodes
  QString path =
      QDir::currentPath() + "/base/themes/" + m_config->theme() + "/gamemodes/";
  for (QString i_folder : QDir(path).entryList(QDir::Dirs))
  {
    if (i_folder == "." || i_folder == "..")
      continue;
    w_gamemode->addItem(i_folder, i_folder);
  }

  // restore previous selection
  w_gamemode->setCurrentText(p_prev_text);

  // unblock
  w_gamemode->blockSignals(false);
}

void AOConfigPanel::refresh_timeofday_list()
{
  const QString p_prev_text = w_timeofday->currentText();

  // block signals
  w_timeofday->blockSignals(true);
  w_timeofday->clear();

  // add empty entry indicating no time of day chosen
  w_timeofday->addItem("<default>");

  // decide path to look for times of day. This differs whether there is a
  // gamemode chosen or not
  QString path;
  if (m_config->gamemode().isEmpty())
    path =
        QDir::currentPath() + "/base/themes/" + m_config->theme() + "/times/";
  else
    path = QDir::currentPath() + "/base/themes/" + m_config->theme() +
           "/gamemodes/" + m_config->gamemode() + "/times/";

  // times of day
  for (QString i_folder : QDir(path).entryList(QDir::Dirs))
  {
    if (i_folder == "." || i_folder == "..")
      continue;
    w_timeofday->addItem(i_folder, i_folder);
  }

  // restore previous selection
  w_timeofday->setCurrentText(p_prev_text);

  // unblock
  w_timeofday->blockSignals(false);
}

void AOConfigPanel::on_reload_theme_clicked()
{
  qDebug() << "reload theme clicked";
  emit reload_theme();
}

void AOConfigPanel::on_gamemode_index_changed(QString p_text)
{
  Q_UNUSED(p_text);
  m_config->set_gamemode(w_gamemode->currentData().toString());
}

void AOConfigPanel::on_timeofday_index_changed(QString p_text)
{
  Q_UNUSED(p_text);
  m_config->set_timeofday(w_timeofday->currentData().toString());
}

void AOConfigPanel::on_log_is_topdown_changed(bool p_enabled)
{
  w_log_orientation_top_down->setChecked(p_enabled);
  w_log_orientation_bottom_up->setChecked(!p_enabled);
}

void AOConfigPanel::on_effects_value_changed(int p_num)
{
  w_effects_value->setText(QString::number(p_num) + "%");
}

void AOConfigPanel::on_system_value_changed(int p_num)
{
  w_system_value->setText(QString::number(p_num) + "%");
}

void AOConfigPanel::on_music_value_changed(int p_num)
{
  w_music_value->setText(QString::number(p_num) + "%");
}

void AOConfigPanel::on_blips_value_changed(int p_num)
{
  w_blips_value->setText(QString::number(p_num) + "%");
}

void AOConfigPanel::on_config_reload_theme_requested()
{
  refresh_theme_list();
  refresh_gamemode_list();
  refresh_timeofday_list();
}
