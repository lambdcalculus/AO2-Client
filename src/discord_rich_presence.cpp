#include "discord_rich_presence.h"
#include "datatypes.h"

#include <cstring>
#include <ctime>

#include <QDebug>

namespace AttorneyOnline
{

Discord::Discord()
{
  // Set up the minimal presence
  minimal_presence->largeImageKey = "danganronpa_online";
  minimal_presence->largeImageText = "Sore Wa Chigau Yo!";
  minimal_presence->instance = 1;
  minimal_presence->state = "";
  minimal_presence->details = "";
  minimal_presence->matchSecret = "";
  minimal_presence->startTimestamp = 0;

  start(APPLICATION_ID);
}

void Discord::start(const char *APPLICATION_ID)
{
  DiscordEventHandlers handlers;
  std::memset(&handlers, 0, sizeof(handlers));
  handlers = {};
  handlers.ready = [](const DiscordUser *user) {
    qInfo() << "Discord RPC ready for" << user->username;
  };
  handlers.disconnected = [](int errorCode, const char *message) {
    Q_UNUSED(errorCode)
    qInfo() << "Discord RPC disconnected! " << message;
  };
  handlers.errored = [](int errorCode, const char *message) {
    Q_UNUSED(errorCode)
    qWarning() << "Discord RPC errored out! " << message;
  };
  qInfo() << "Initializing Discord RPC";
  Discord_Initialize(APPLICATION_ID, &handlers, 1, nullptr);
}

Discord::~Discord()
{
  Discord_Shutdown();
}

void Discord::restart(const char *APPLICATION_ID)
{
  Discord_Shutdown();
  start(APPLICATION_ID);
}

void Discord::set_style(DR::DiscordRichPresenceStyle new_style)
{
  style = new_style;
  refresh_presence();
}

void Discord::refresh_presence()
{
  switch (style)
  {
  case DR::DRPSComplete:
    // Don't modify presence from its current status
    Discord_UpdatePresence(complete_presence);
    break;
  case DR::DRPSMinimal:
    // Use instead minimal presence
    Discord_UpdatePresence(minimal_presence);
    break;
  case DR::DRPSDisabled:
    // Wipe out
    Discord_ClearPresence();
    break;
  }
  // Note that, no matter what, complete_presence always holds a
  // fully updated presence, so it is safe to use it any moment
}

void Discord::state_lobby()
{
  this->server_id = QString("").toStdString().c_str();
  this->server_name = QString("").toStdString().c_str();

  this->details = "In Lobby";
  this->state = "Idle";
  this->timestamp = 0;
  this->matchSecret = QString("").toStdString().c_str();

  complete_presence = new DiscordRichPresence();

  complete_presence->largeImageKey = "danganronpa_online";
  complete_presence->largeImageText = "Sore Wa Chigau Yo!";
  complete_presence->instance = 1;

  complete_presence->details = this->details.c_str();
  complete_presence->state = this->state.c_str();

  refresh_presence();
}

void Discord::state_server(std::string name, std::string server_id)
{
  qDebug() << "Discord RPC: Setting server state";

  auto timestamp = static_cast<int64_t>(std::time(nullptr));

  this->server_id = server_id;
  this->server_name = name;

  this->details = this->server_name;
  this->state = "Connecting...";
  this->matchSecret = this->server_id;
  this->timestamp = timestamp;

  complete_presence = new DiscordRichPresence();

  complete_presence->largeImageKey = "danganronpa_online";
  complete_presence->largeImageText = "Sore Wa Chigau Yo!";
  complete_presence->instance = 1;

  complete_presence->details = this->details.c_str();
  complete_presence->state = this->state.c_str();
  complete_presence->matchSecret = this->matchSecret.c_str();
  complete_presence->startTimestamp = this->timestamp;

  refresh_presence();
}

void Discord::state_character(std::string name)
{
  // auto name_internal = QString(name.c_str()).toLower().replace(' ', '_').toStdString();
  auto name_friendly = QString(name.c_str()).replace('_', ' ').toStdString();
  const std::string playing_as = "Playing as " + name_friendly;
  qDebug() << "Discord RPC: Setting character state (" << playing_as.c_str() << ")";

  this->details = this->server_name;
  this->state = playing_as.c_str();

  complete_presence = new DiscordRichPresence();

  complete_presence->largeImageKey = "danganronpa_online";
  complete_presence->largeImageText = "Sore Wa Chigau Yo!";
  complete_presence->instance = 1;

  complete_presence->details = this->details.c_str();
  complete_presence->state = this->state.c_str();
  complete_presence->matchSecret = this->matchSecret.c_str();
  complete_presence->startTimestamp = this->timestamp;

  refresh_presence();
}

void Discord::state_spectate()
{
  qDebug() << "Discord RPC: Setting spectator state";

  this->details = this->server_name;
  this->state = "Spectating";

  complete_presence = new DiscordRichPresence();

  complete_presence->largeImageKey = "danganronpa_online";
  complete_presence->largeImageText = "Sore Wa Chigau Yo!";
  complete_presence->instance = 1;

  complete_presence->details = this->details.c_str();
  complete_presence->state = this->state.c_str();
  complete_presence->matchSecret = this->matchSecret.c_str();
  complete_presence->startTimestamp = this->timestamp;

  refresh_presence();
}

} // namespace AttorneyOnline
