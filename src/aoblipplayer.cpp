#include "aoblipplayer.h"

AOBlipPlayer::AOBlipPlayer(AOApplication *p_ao_app, QObject *p_parent) : AOObject(p_ao_app, p_parent)
{
  m_family = DRAudioEngine::get_family(DRAudio::Family::FBlip);
  m_family->set_capacity(BLIP_COUNT);
}

void AOBlipPlayer::set_blips(QString p_blip)
{
  if (m_blip.has_value() && m_blip.value() == p_blip)
    return;

  m_blip = p_blip;
  m_file = ao_app->get_sounds_path(m_blip.value());
}

void AOBlipPlayer::blip_tick()
{
  if (!m_file.has_value())
    return;
  m_family->play_stream(m_file.value());
}
