#ifndef DISCORD_RICH_PRESENCE_H
#define DISCORD_RICH_PRESENCE_H
// 3rd
#include <discord_rpc.h>
// std
#include <string>

#include "datatypes.h"

namespace AttorneyOnline
{

class Discord
{
private:
  const char *APPLICATION_ID = "818850172330442793";
  int m_index = 0;
  std::string server_name, server_id, state, details, matchSecret;
  int64_t timestamp;
  DR::DiscordRichPresenceStyle style;
  DiscordRichPresence *complete_presence = new DiscordRichPresence();
  DiscordRichPresence *minimal_presence = new DiscordRichPresence();
  void refresh_presence();

public:
  Discord();
  ~Discord();

  void state_lobby();
  void state_server(std::string name, std::string server_id);
  void state_character(std::string name);
  void state_spectate();
  void start(const char *APPLICATION_ID);
  void restart(const char *APPLICATION_ID);
  void set_style(DR::DiscordRichPresenceStyle new_style);
};

} // namespace AttorneyOnline
#endif // DISCORD_RICH_PRESENCE_H
