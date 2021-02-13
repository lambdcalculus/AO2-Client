#include "courtroom.h"

bool Courtroom::is_audio_muted()
{
  return m_audio_mute;
}

void Courtroom::set_audio_mute_enabled(bool p_enabled)
{
  if (m_audio_mute == p_enabled)
    return;
  m_audio_mute = p_enabled;

  // suppress audio
  for (auto &family : DRAudioEngine::get_family_list())
    family->set_suppressed(m_audio_mute);
}
