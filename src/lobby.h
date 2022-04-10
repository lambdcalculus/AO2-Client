#ifndef LOBBY_H
#define LOBBY_H

#include "datatypes.h"

class AOApplication;
class AOButton;
class AOConfig;
class AOImageDisplay;
class DRChatLog;
class DRMasterClient;
class DRTextEdit;

#include <QMainWindow>
#include <QModelIndex>

class QListWidget;
class QLineEdit;
class QProgressBar;
class QTextBrowser;

class Lobby : public QMainWindow
{
  Q_OBJECT

public:
  Lobby(AOApplication *p_ao_app);
  ~Lobby();

  DRServerInfoList get_combined_server_list();

  void set_choose_a_server();
  void set_player_count(int players_online, int max_players);
  void set_loading_text(QString p_text);
  void set_stylesheet(QWidget *widget, QString target_tag);
  void set_stylesheets();
  void set_fonts();
  void show_loading_overlay();
  void hide_loading_overlay();
  DRServerInfo get_selected_server();
  void set_loading_value(int p_value);

signals:
  void server_list_changed();
  void favorite_server_list_changed();
  void combined_server_list_changed();

private:
  AOApplication *ao_app = nullptr;
  AOConfig *ao_config = nullptr;

  DRMasterClient *m_master_client = nullptr;
  DRServerInfoList m_server_list;
  DRServerInfoList m_favorite_server_list;
  DRServerInfoList m_combined_server_list;
  DRServerInfo m_current_server;
  bool is_public_server_selected = true;

  // ui
  AOImageDisplay *ui_background = nullptr;
  AOButton *ui_hide_public_servers = nullptr;
  bool m_hide_public_servers = false;
  AOButton *ui_hide_favorite_servers = nullptr;
  bool m_hide_favorite_servers = false;
  AOButton *ui_refresh = nullptr;
  AOButton *ui_toggle_favorite = nullptr;
  AOButton *ui_connect = nullptr;
  DRTextEdit *ui_version = nullptr;
  AOButton *ui_about = nullptr;
  AOButton *ui_config = nullptr;
  QListWidget *ui_server_list = nullptr;
  DRTextEdit *ui_player_count = nullptr;
  QTextBrowser *ui_description = nullptr;
  DRChatLog *ui_chatbox = nullptr;
  AOImageDisplay *ui_loading_background = nullptr;
  DRTextEdit *ui_loading_text = nullptr;
  QProgressBar *ui_progress_bar = nullptr;
  AOButton *ui_cancel = nullptr;

  void load_settings();
  void save_settings();

  void load_favorite_server_list();
  void load_legacy_favorite_server_list();
  void save_favorite_server_list();

private slots:
  void update_widgets();

  void request_advertiser_update();
  void update_motd();
  void update_server_list();
  void set_favorite_server_list(DRServerInfoList server_list);
  void update_combined_server_list();
  void hide_public_servers(bool on);
  void toggle_hide_public_servers();
  void hide_favorite_servers(bool on);
  void toggle_hide_favorite_servers();
  void update_server_listing();
  void filter_server_listing();
  void select_current_server();

  void on_refresh_pressed();
  void on_refresh_released();
  void on_add_to_fav_pressed();
  void on_add_to_fav_released();
  void on_connect_pressed();
  void on_connect_released();
  void on_config_pressed();
  void on_config_released();
  void on_about_clicked();
  void connect_to_server(int row);
};

#endif // LOBBY_H
