#include "draudiostream.h"

#include <QDebug>
#include <QFileInfo>
#include <QtMath>

#include <bass.h>
#include <bassopus.h>

#include "draudioengine.h"
#include "draudiostreamfamily.h"

DRAudioStream::DRAudioStream(DRAudio::Family p_family) : m_engine(new DRAudioEngine(this)), m_family(p_family)
{
  connect(m_engine, &DRAudioEngine::current_device_changed, this, &DRAudioStream::update_device);
}

DRAudioStream::~DRAudioStream()
{
  if (m_hstream)
  {
    BASS_StreamFree(m_hstream);
  }
}

DRAudio::Family DRAudioStream::get_family() const
{
  return m_family;
}

QString DRAudioStream::get_file_name() const
{
  return m_file_name;
}

bool DRAudioStream::is_playing() const
{
  if (!ensure_init())
    return false;
  return BASS_ChannelIsActive(m_hstream) == BASS_ACTIVE_PLAYING;
}

bool DRAudioStream::is_repeatable() const
{
  return m_repeatable;
}

void DRAudioStream::play()
{
  if (!ensure_init())
    return;
  if (!BASS_ChannelPlay(m_hstream, FALSE))
  {
    qWarning()
        << QString("error: failed to play file: %1 (file: \"%1\")").arg(DRAudio::get_last_bass_error(), m_file_name);
    Q_EMIT finished();
  }
}

void DRAudioStream::stop()
{
  if (!ensure_init())
    return;
  BASS_ChannelStop(m_hstream);
  Q_EMIT finished();
}

std::optional<DRAudioError> DRAudioStream::set_file_name(QString p_file_name)
{
  m_file_name = p_file_name;
  m_init_state = InitNotDone;
  if (!ensure_init())
  {
    return DRAudioError("failed to set file");
  }
  emit file_name_changed(m_file_name);
  return std::nullopt;
}

void DRAudioStream::set_volume(float p_volume)
{
  if (!ensure_init())
    return;
  m_volume = p_volume;
  BASS_ChannelSetAttribute(m_hstream, BASS_ATTRIB_VOL, float(p_volume) * 0.01f);
}

void DRAudioStream::set_repeatable(bool p_enabled)
{
  if (m_repeatable == p_enabled)
    return;
  m_repeatable = p_enabled;
  init_loop();
}

void DRAudioStream::set_loop(quint64 p_start, quint64 p_end)
{
  if (!ensure_init())
    return;

  if (m_loop_start == p_start && m_loop_end == p_end)
    return;
  m_loop_start = p_start;
  m_loop_end = p_end;
  init_loop();
}

void DRAudioStream::end_sync(HSYNC hsync, DWORD ch, DWORD data, void *userdata)
{
  Q_UNUSED(hsync);
  Q_UNUSED(ch);
  Q_UNUSED(data);

  DRAudioStream *l_stream = static_cast<DRAudioStream *>(userdata);
  if (!l_stream->is_repeatable())
  {
    emit l_stream->finished();
  }
}

void DRAudioStream::loop_sync(HSYNC hsync, DWORD ch, DWORD data, void *userdata)
{
  Q_UNUSED(hsync);
  Q_UNUSED(data);

  // move the position to the loopStart
  DRAudioStream *l_stream = static_cast<DRAudioStream *>(userdata);
  if (l_stream->is_repeatable())
  {
    BASS_ChannelSetPosition(ch, l_stream->m_loop_start_pos, BASS_POS_BYTE);
    emit l_stream->looped();
  }
}

bool DRAudioStream::ensure_init()
{
  if (m_init_state != InitNotDone)
    return m_init_state == InitFinished;
  m_init_state = InitError;

  if (m_file_name.isEmpty())
    return false;

  HSTREAM l_hstream;
  if (m_file_name.endsWith("opus", Qt::CaseInsensitive))
    l_hstream = BASS_OPUS_StreamCreateFile(FALSE, m_file_name.utf16(), 0, 0, BASS_UNICODE | BASS_ASYNCFILE);
  else
    l_hstream =
        BASS_StreamCreateFile(FALSE, m_file_name.utf16(), 0, 0, BASS_UNICODE | BASS_ASYNCFILE | BASS_STREAM_PRESCAN);

  if (!l_hstream)
  {
    qWarning() << "error: failed to create audio stream (file:" << m_file_name << ")";
    return false;
  }
  m_hstream = l_hstream;

  BASS_ChannelSetSync(l_hstream, BASS_SYNC_END | BASS_SYNC_MIXTIME, 0, &end_sync, this);
  m_init_state = InitFinished;
  init_loop();
  return true;
}

bool DRAudioStream::ensure_init() const
{
  return const_cast<DRAudioStream *>(this)->ensure_init();
}

void DRAudioStream::init_loop()
{
  if (m_loop_sync)
  {
    BASS_ChannelRemoveSync(m_hstream, m_loop_sync);
    m_loop_sync = 0;
  }

  if (m_repeatable)
  {

    float l_sample_rate;
    BASS_ChannelGetAttribute(m_hstream, BASS_ATTRIB_FREQ, &l_sample_rate);

    const QWORD l_length = BASS_ChannelGetLength(m_hstream, BASS_POS_BYTE);
    m_loop_start_pos = BASS_ChannelSeconds2Bytes(m_hstream, m_loop_start / double(l_sample_rate));
    if (m_loop_start_pos >= l_length)
    {
      m_loop_start_pos = 0;
    }

    m_loop_end_pos = BASS_ChannelSeconds2Bytes(m_hstream, m_loop_end / double(l_sample_rate));
    if (m_loop_end_pos <= m_loop_start_pos || m_loop_end_pos > l_length)
    {
      m_loop_end_pos = l_length;
    }

    m_loop_sync = BASS_ChannelSetSync(m_hstream, BASS_SYNC_POS | BASS_SYNC_MIXTIME, m_loop_end_pos, &loop_sync, this);
  }
}

void DRAudioStream::update_device(DRAudioDevice p_device)
{
  if (!ensure_init())
    return;
  if (BASS_ChannelGetDevice(m_hstream) != p_device.get_id())
  {
    if (!BASS_ChannelSetDevice(m_hstream, p_device.get_id()))
    {
      qDebug() << "error: failed to switch stream device;" << DRAudio::get_last_bass_error() << p_device.get_name();
    }
  }
}