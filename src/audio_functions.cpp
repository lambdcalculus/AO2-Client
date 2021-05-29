#include "courtroom.h"

#include "draudioengine.h"

bool Courtroom::is_audio_suppressed() const
{
  return m_audio_mute;
}

void Courtroom::suppress_audio(bool p_enabled)
{
  if (m_audio_mute == p_enabled)
    return;
  m_audio_mute = p_enabled;

  // suppress audio
  for (auto &family : DRAudioEngine::get_family_list())
    family->set_suppressed(m_audio_mute);
}

void Courtroom::stop_all_audio()
{
  for (auto &family : DRAudioEngine::get_family_list())
    for (auto &stream : family->get_stream_list())
      stream->stop();
}
