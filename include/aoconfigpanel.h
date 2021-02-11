#ifndef AOCONFIGPANEL_H
#define AOCONFIGPANEL_H

// qt
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QTabWidget>
#include <QWidget>

// src
#include "aoconfig.h"
#include "aoguiloader.h"

class AOApplication;

class AOConfigPanel : public QWidget
{
  Q_OBJECT

public:
  AOConfigPanel(AOApplication *p_ao_app, QWidget *p_parent = nullptr);

public slots:
  void on_config_reload_theme_requested();

protected:
  void showEvent(QShowEvent *event);

signals:
  void reload_theme();

private:
  void refresh_theme_list();
  void refresh_gamemode_list();
  void refresh_timeofday_list();

private slots:
  void on_reload_theme_clicked();
  void on_gamemode_index_changed(QString p_text);
  void on_timeofday_index_changed(QString p_text);
  void on_log_is_topdown_changed(bool p_enabled);
  void on_master_value_changed(int p_num);
  void on_system_value_changed(int p_num);
  void on_effect_value_changed(int p_num);
  void on_music_value_changed(int p_num);
  void on_blip_value_changed(int p_num);

private:
  // FIXME This dependency shouldn't have come to exist.
  AOApplication *ao_app = nullptr;

  // driver
  AOConfig *m_config = nullptr;

  // behaviour
  QPushButton *w_save = nullptr;
  QPushButton *w_close = nullptr;
  QCheckBox *w_autosave = nullptr;

  // general
  QLineEdit *w_username = nullptr;
  QLineEdit *w_callwords = nullptr;
  QCheckBox *w_server_alerts = nullptr;

  // game
  QComboBox *w_theme = nullptr;
  QPushButton *w_reload_theme = nullptr;
  QComboBox *w_gamemode = nullptr;
  QCheckBox *w_manual_gamemode = nullptr;
  QComboBox *w_timeofday = nullptr;
  QCheckBox *w_manual_timeofday = nullptr;
  QCheckBox *w_always_pre = nullptr;
  QSpinBox *w_chat_tick_interval = nullptr;

  // IC Chatlog
  QSpinBox *w_log_max_lines = nullptr;
  QCheckBox *w_log_uses_newline = nullptr;
  QRadioButton *w_log_orientation_top_down = nullptr;
  QRadioButton *w_log_orientation_bottom_up = nullptr;
  QCheckBox *w_log_music = nullptr;
  QCheckBox *w_log_is_recording = nullptr;

  // audio
  QSlider *w_master = nullptr;
  QLabel *w_master_value = nullptr;
  QCheckBox *w_disable_background_audio = nullptr;
  QSlider *w_system = nullptr;
  QLabel *w_system_value = nullptr;
  QSlider *w_effect = nullptr;
  QLabel *w_effect_value = nullptr;
  QSlider *w_music = nullptr;
  QLabel *w_music_value = nullptr;
  QSlider *w_blip = nullptr;
  QLabel *w_blip_value = nullptr;
  QSpinBox *w_blip_rate = nullptr;
  QCheckBox *w_blank_blips = nullptr;
};

#endif // AOCONFIGPANEL_H
