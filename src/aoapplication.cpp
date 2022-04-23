#include "aoapplication.h"

#include "aoconfig.h"
#include "aoconfigpanel.h"
#include "courtroom.h"
#include "debug_functions.h"
#include "drdiscord.h"
#include "drmasterclient.h"
#include "drpacket.h"
#include "drserversocket.h"
#include "lobby.h"
#include "theme.h"
#include "version.h"

#include <QDir>
#include <QFontDatabase>
#include <QRegularExpression>

AOApplication::AOApplication(int &argc, char **argv) : QApplication(argc, argv)
{
  ao_config = new AOConfig(this);
  ao_config_panel = new AOConfigPanel(this);
  dr_discord = new DRDiscord(this);
  m_server_socket = new DRServerSocket(this);

  connect(ao_config, SIGNAL(theme_changed(QString)), this, SLOT(handle_theme_modification()));
  connect(ao_config, SIGNAL(gamemode_changed(QString)), this, SLOT(handle_theme_modification()));
  connect(ao_config, SIGNAL(timeofday_changed(QString)), this, SLOT(handle_theme_modification()));
  connect(ao_config, SIGNAL(manual_gamemode_changed(QString)), this, SLOT(handle_theme_modification()));
  connect(ao_config, SIGNAL(manual_gamemode_selection_changed(bool)), this, SLOT(handle_theme_modification()));
  connect(ao_config, SIGNAL(manual_timeofday_changed(QString)), this, SLOT(handle_theme_modification()));
  connect(ao_config, SIGNAL(manual_timeofday_selection_changed(bool)), this, SLOT(handle_theme_modification()));
  connect(ao_config_panel, SIGNAL(reload_theme()), this, SLOT(handle_theme_modification()));
  connect(ao_config_panel, SIGNAL(reload_character()), this, SLOT(handle_character_reloading()));
  connect(ao_config_panel, SIGNAL(reload_audiotracks()), this, SLOT(handle_audiotracks_reloading()));
  ao_config_panel->hide();

  dr_discord->set_presence(ao_config->discord_presence());
  dr_discord->set_hide_server(ao_config->discord_hide_server());
  dr_discord->set_hide_character(ao_config->discord_hide_character());
  connect(ao_config, SIGNAL(discord_presence_changed(bool)), dr_discord, SLOT(set_presence(bool)));
  connect(ao_config, SIGNAL(discord_hide_server_changed(bool)), dr_discord, SLOT(set_hide_server(bool)));
  connect(ao_config, SIGNAL(discord_hide_character_changed(bool)), dr_discord, SLOT(set_hide_character(bool)));

  connect(m_server_socket, SIGNAL(disconnected_from_server()), this, SLOT(_p_handle_server_disconnection()));
  connect(m_server_socket, SIGNAL(packet_received(DRPacket)), this, SLOT(_p_handle_server_packet(DRPacket)));
}

AOApplication::~AOApplication()
{
  qInfo() << "Closing Danganronpa Online...";
  destruct_lobby();
  destruct_courtroom();
}

int AOApplication::get_client_id() const
{
  return m_client_id;
}

void AOApplication::set_client_id(int id)
{
  m_client_id = id;
}

Lobby *AOApplication::get_lobby() const
{
  return m_lobby;
}

void AOApplication::construct_lobby()
{
  if (is_lobby_constructed)
  {
    qDebug() << "W: lobby was attempted constructed when it already exists";
    return;
  }

  m_lobby = new Lobby(this);
  is_lobby_constructed = true;
  center_widget_to_screen(m_lobby);
  m_lobby->show();

  dr_discord->set_state(DRDiscord::State::Idle);
  dr_discord->clear_server_name();
}

void AOApplication::destruct_lobby()
{
  if (!is_lobby_constructed)
  {
    qDebug() << "W: lobby was attempted destructed when it did not exist";
    return;
  }

  delete m_lobby;
  is_lobby_constructed = false;
}

Courtroom *AOApplication::get_courtroom() const
{
  return m_courtroom;
}

void AOApplication::construct_courtroom()
{
  if (is_courtroom_constructed)
  {
    qDebug() << "W: courtroom was attempted constructed when it already exists";
    return;
  }

  m_courtroom = new Courtroom(this);
  connect(m_courtroom, SIGNAL(closing()), this, SLOT(on_courtroom_closing()));
  connect(m_courtroom, SIGNAL(destroyed()), this, SLOT(on_courtroom_destroyed()));
  is_courtroom_constructed = true;
  center_widget_to_screen(m_courtroom);
}

void AOApplication::destruct_courtroom()
{
  // destruct courtroom
  if (is_courtroom_constructed)
  {
    delete m_courtroom;
    is_courtroom_constructed = false;
    ao_config->set_gamemode(nullptr);
    ao_config->set_timeofday(nullptr);
  }
  else
  {
    qDebug() << "W: courtroom was attempted destructed when it did not exist";
  }

  // gracefully close our connection to the current server
  m_server_socket->disconnect_from_server();
}

DRDiscord *AOApplication::get_discord() const
{
  return dr_discord;
}

VersionNumber AOApplication::get_server_client_version() const
{
  return m_server_client_version;
}

VersionStatus AOApplication::get_server_client_version_status() const
{
  return m_server_client_version_status;
}

bool AOApplication::is_server_client_version_compatible() const
{
  return m_server_client_version_status == VersionStatus::Ok;
}

void AOApplication::handle_theme_modification()
{
  load_fonts();
  emit reload_theme();
}

void AOApplication::handle_character_reloading()
{
  emit reload_character();
}

void AOApplication::handle_audiotracks_reloading()
{
  emit reload_audiotracks();
}

QString AOApplication::get_current_char()
{
  if (!is_courtroom_constructed)
    return nullptr;
  return m_courtroom->get_character_ini();
}

/**
 * @brief Check the path for various known exploits.
 *
 * In order:
 * - Directory traversal (most commonly: "../" jumps)
 * @param p_file The path to check.
 * @return A sanitized path. If any check fails, the path returned is an empty string. The sanitized path does not
 * necessarily exist.
 */
bool AOApplication::is_safe_path(QString p_file)
{
  if (!p_file.contains(".."))
    return true;
  const QStringList l_item_list = p_file.split(QRegularExpression("[\\/]"));
  for (auto it = l_item_list.crbegin(); it != l_item_list.crend(); ++it)
    if (*it == "..")
      return false;
  return true;
}

void AOApplication::toggle_config_panel()
{
  ao_config_panel->setVisible(!ao_config_panel->isVisible());
  if (ao_config_panel->isVisible())
  {
    ao_config_panel->setFocus();
    ao_config_panel->raise();
    center_widget_to_screen(ao_config_panel);
  }
}

bool AOApplication::get_first_person_enabled()
{
  return ao_config->get_bool("first_person", false);
}

void AOApplication::load_fonts()
{
  QFontDatabase l_database;
  for (const QFileInfo &fileInfo : QDir(get_case_sensitive_path(get_base_path() + "fonts")).entryInfoList())
    l_database.addApplicationFont(fileInfo.absoluteFilePath());
}

void AOApplication::loading_cancelled()
{
  destruct_courtroom();

  m_lobby->hide_loading_overlay();
}

void AOApplication::on_courtroom_closing()
{
  ao_config_panel->hide();
}

void AOApplication::on_courtroom_destroyed()
{
  ao_config_panel->hide();
}
