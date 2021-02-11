#include "draudiostream.h"

#include "draudioengine.h"
#include "draudiostreamfamily.h"

#include <QDebug>
#include <QFileInfo>

DRAudioStream::DRAudioStream(DRAudio::Family p_family) : m_family(p_family)
{}

DRAudioStream::~DRAudioStream()
{
  if (m_hstream.has_value())
  {
    const HSTREAM hstream = m_hstream.value();

    while (!m_hsync_stack.empty())
    {
      BASS_ChannelRemoveSync(hstream, m_hsync_stack.top());
      m_hsync_stack.pop();
    }

    BASS_StreamFree(hstream);
  }
}

DRAudio::Family DRAudioStream::get_family()
{
  return m_family;
}

std::optional<QString> DRAudioStream::get_file()
{
  return m_file;
}

bool DRAudioStream::is_playing()
{
  if (!m_hstream)
    return false;
  return BASS_ChannelIsActive(m_hstream.value()) == BASS_ACTIVE_PLAYING;
}

void DRAudioStream::play()
{
  if (!m_hstream)
    return;
  const BOOL result = BASS_ChannelPlay(m_hstream.value(), FALSE);
  if (result == FALSE)
  {
    qWarning() << DRAudioError(
                      QString("failed to play file %1: %2").arg(m_file.value()).arg(DRAudio::get_last_bass_error()))
                      .get_error();
    Q_EMIT finished();
  }
}

void DRAudioStream::stop()
{
  if (!m_hstream)
    return;
  BASS_ChannelStop(m_hstream.value());
  Q_EMIT finished();
}

std::optional<DRAudioError> DRAudioStream::set_file(QString p_file)
{
  if (m_file.has_value())
    return DRAudioError("file already set");

  const QFileInfo file(p_file);
  if (!file.exists())
    return DRAudioError("file does not exist");

  if (m_file == p_file)
    return std::nullopt;

  m_file = p_file;

  HSTREAM stream_handle = BASS_StreamCreateFile(FALSE, m_file->utf16(), 0, 0, BASS_UNICODE | BASS_ASYNCFILE);
  if (stream_handle == 0)
    return DRAudioError(
        QString("failed to create stream for file %1: %2").arg(p_file).arg(DRAudio::get_last_bass_error()));
  m_hstream = stream_handle;

  // bass events
  HSYNC sync_handle = BASS_ChannelSetSync(stream_handle, BASS_SYNC_END, 0, &DRAudioStream::sync_on_end, this);
  if (sync_handle == 0)
    return DRAudioError(
        QString("failed to create sync for file %1: %2").arg(p_file).arg(DRAudio::get_last_bass_error()));

  Q_EMIT file_changed(p_file);
  return std::nullopt;
}

void DRAudioStream::set_volume(int32_t p_volume)
{
  if (!m_hstream)
    return;
  BASS_ChannelSetAttribute(m_hstream.value(), BASS_ATTRIB_VOL, float(p_volume) * 0.01f);
}

void DRAudioStream::sync_on_end(HSYNC hsync, DWORD ch, DWORD data, void *userdata)
{
  Q_UNUSED(hsync)
  Q_UNUSED(ch)
  Q_UNUSED(data)

  /*
   * BASS only provides what we fed it when we first created our synch even if
   * the pointer we provided get deleted mid-way through the program lifetime
   */
  DRAudioStream *self = static_cast<DRAudioStream *>(userdata);
  Q_EMIT self->finished();
}
