#include "aomusicplayer.h"

#include "aoapplication.h"

#include "draudiotrackmetadata.h"
#include <QDebug>

AOMusicPlayer::AOMusicPlayer(AOApplication *p_ao_app, QObject *p_parent) : AOObject(p_ao_app, p_parent)
{
  m_family = DRAudioEngine::get_family(DRAudio::Family::FMusic);
  m_family->set_capacity(1); // a single song is needed
}

void AOMusicPlayer::play(QString p_song)
{
  stop();

  m_filename = p_song;
  auto l_maybe_stream = m_family->create_stream(p_song);
  if (l_maybe_stream)
  {
    auto l_stream = l_maybe_stream.value();
    DRAudiotrackMetadata l_audiotrack(p_song);
    if (!l_audiotrack.play_once())
    {
      l_stream->set_repeatable(true);
      l_stream->set_loop(l_audiotrack.loop_start(), l_audiotrack.loop_end());
    }
    l_stream->play();

    if (l_stream->is_playing())
    {
      qDebug() << "playing" << l_stream->get_file_name();
    }
  }
}

void AOMusicPlayer::stop()
{
  for (auto &song : m_family->get_stream_list())
    song->stop();
}
