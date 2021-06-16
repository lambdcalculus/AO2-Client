#include "courtroom.h"

#include "aoconfig.h"

int Courtroom::get_character_id()
{
  return m_chr_id;
}

QString Courtroom::get_base_character()
{
  return is_spectating() ? nullptr : m_chr_list.at(m_chr_id).name;
}

QString Courtroom::get_current_character()
{
  return ao_config->character_ini(get_base_character());
}
