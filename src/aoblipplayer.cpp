#include "aoblipplayer.h"

AOBlipPlayer::AOBlipPlayer(AOApplication *p_ao_app, QObject *p_parent) : AOObject(p_ao_app, p_parent)
{
  m_family = DRAudioEngine::get_family(DRAudio::Family::FBlip);
  m_family->set_capacity(BLIP_COUNT);
}

void AOBlipPlayer::set_blips(QString p_blip)
{
  if (m_blipName.has_value() && m_blipName.value() == p_blip)
    return;

  m_blipName = p_blip;
  m_blipFile = ao_app->get_sounds_path(m_blipName.value());
}

void AOBlipPlayer::blip_tick()
{
  if (!m_blipFile.has_value())
    return;
  m_family->play_stream(m_blipFile.value());
}
