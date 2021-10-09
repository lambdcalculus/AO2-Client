#include "draudiostream.h"

#include "draudioengine.h"
#include "draudiostreamfamily.h"

#include <bassopus.h>

#include <QDebug>
#include <QFileInfo>

DRAudioStream::DRAudioStream(DRAudio::Family p_family) : m_family(p_family)
{
  connect(this, &DRAudioStream::device_error, this, &DRAudioStream::on_device_error, Qt::QueuedConnection);
}

DRAudioStream::~DRAudioStream()
{
  if (m_hstream.has_value())
  {
    const HSTREAM hstream = m_hstream.value();

    while (!m_hsync_stack.empty())
    {
      BASS_ChannelRemoveSync(hstream, m_hsync_stack.top().sync);
      m_hsync_stack.pop();
    }

    BASS_StreamFree(hstream);
  }
}

DRAudio::Family DRAudioStream::get_family() const
{
  return m_family;
}

std::optional<QString> DRAudioStream::get_file() const
{
  return m_file;
}

bool DRAudioStream::is_playing() const
{
  if (!m_hstream.has_value())
    return false;
  return BASS_ChannelIsActive(m_hstream.value()) == BASS_ACTIVE_PLAYING;
}

void DRAudioStream::play()
{
  if (!m_hstream.has_value())
    return;
  const BOOL result = BASS_ChannelPlay(m_hstream.value(), FALSE);
  if (result == FALSE)
  {
    qWarning() << DRAudioError(
                      QString("failed to play file %1: %2").arg(m_file.value()).arg(DRAudio::get_last_bass_error()))
                      .what();
    Q_EMIT finished();
  }
  setup_looping();
}

void DRAudioStream::stop()
{
  if (!m_hstream.has_value())
    return;
  BASS_ChannelStop(m_hstream.value());
  Q_EMIT finished();
}

QWORD DRAudioStream::loop_start()
{
  return m_loop_start;
}

QWORD DRAudioStream::loop_end()
{
  return m_loop_end;
}

// the sync callback
static void CALLBACK loop_sync(unsigned long syncHandle, unsigned long channel, unsigned long data, void* user)
{
  Q_UNUSED(syncHandle);
  Q_UNUSED(data);

  // move the position to the loopStart
  DRAudioStream *stream = static_cast<DRAudioStream*>(user);
  QWORD loop_start = stream->loop_start();
  BASS_ChannelSetPosition(channel, loop_start, BASS_POS_BYTE);
}

void DRAudioStream::setup_looping()
{
  // Remove all previously set loop information
  m_loop_start = 0;
  m_loop_end = 0;

  if (m_loop_sync > 0)
  {
    BASS_ChannelRemoveSync(m_hstream.value(), m_loop_sync);
    m_loop_sync = 0;
  }

  // Right now we only support files that satisfy the following:
  // 1. They are OGG files
  // 2. They indicate a positive sample rate
  // 3. They have, as OGG comments, positive values of LoopStart and LoopEnd (denoted as an int in samples)
  // 4. LoopStart <= LoopEnd

  if (!m_file->endsWith("ogg", Qt::CaseInsensitive))
    return;

  float sample_rate = 0;
  if (!BASS_ChannelGetAttribute(m_hstream.value(), BASS_ATTRIB_FREQ, &sample_rate))
    return;
  if (sample_rate == 0)
    return;
  // Now sample_rate holds the sample rate in Hertz

  const char* ogg_value = BASS_ChannelGetTags(m_hstream.value(), BASS_TAG_OGG);
  QStringList ogg_comments;
  while (*ogg_value)
  {
    ogg_comments.push_back(QString(ogg_value));
    ogg_value += ogg_comments.back().size() + 1;
  }

  float loop_start = 0;
  float loop_end = 0;
  for (const QString &ogg_comment : ogg_comments)
  {
    QStringList split = ogg_comment.split('=');
    if (split.size() == 0)
      continue;
    if (split.at(0) == "LoopStart")
      loop_start = split.at(1).toFloat();
    else if (split.at(0) == "LoopEnd")
      loop_end = split.at(1).toFloat();
  }
  if (loop_start > loop_end || (loop_start == 0 && loop_end == 0))
    return;

  // If we are at this point, we are successful in fetching all required values

  m_loop_start = BASS_ChannelSeconds2Bytes(m_hstream.value(), loop_start / sample_rate);
  m_loop_end = BASS_ChannelSeconds2Bytes(m_hstream.value(), loop_end / sample_rate);

  m_loop_sync = BASS_ChannelSetSync(m_hstream.value(), BASS_SYNC_POS | BASS_SYNC_MIXTIME,
                                    m_loop_end, loop_sync, this);
}

std::optional<DRAudioError> DRAudioStream::set_file(QString p_file)
{
  if (m_file.has_value())
    return DRAudioError("file already set");

  const QFileInfo file(p_file);
  if (!file.exists())
    return DRAudioError(QString("file does not exist: %1").arg(p_file.isEmpty() ? "<empty>" : p_file));

  if (m_file == p_file)
    return std::nullopt;

  m_file = p_file;

  HSTREAM stream_handle;
  if (m_file->endsWith("opus", Qt::CaseInsensitive))
    stream_handle = BASS_OPUS_StreamCreateFile(FALSE, m_file->utf16(), 0, 0, BASS_UNICODE | BASS_ASYNCFILE);
  else
    stream_handle = BASS_StreamCreateFile(FALSE, m_file->utf16(), 0, 0, BASS_UNICODE | BASS_ASYNCFILE);

  if (stream_handle == 0)
    return DRAudioError(
        QString("failed to create stream for file %1: %2").arg(p_file).arg(DRAudio::get_last_bass_error()));
  m_hstream = stream_handle;

  // bass events
  for (const DWORD type : {BASS_SYNC_END, BASS_SYNC_DEV_FAIL})
  {
    const DWORD final_type = type == BASS_SYNC_DEV_FAIL ? type | BASS_SYNC_MIXTIME : type;
    HSYNC sync_handle = BASS_ChannelSetSync(stream_handle, final_type, 0, &DRAudioStream::on_sync_callback, this);
    if (sync_handle == 0)
      return DRAudioError(
          QString("failed to create sync %1 for file %2: %3").arg(type).arg(p_file, DRAudio::get_last_bass_error()));
    m_hsync_stack.push(DRAudioStreamSync{sync_handle, type});
  }

  Q_EMIT file_changed(p_file);
  return std::nullopt;
}

void DRAudioStream::set_volume(float p_volume)
{
  if (!m_hstream.has_value())
    return;
  m_volume = p_volume;
  BASS_ChannelSetAttribute(m_hstream.value(), BASS_ATTRIB_VOL, float(p_volume) * 0.01f);
}

#include <QTimer>

void DRAudioStream::on_sync_callback(HSYNC hsync, DWORD ch, DWORD data, void *userdata)
{
  Q_UNUSED(hsync)
  Q_UNUSED(ch)
  Q_UNUSED(data)

  /*
   * BASS only provides what we fed it when we first created our synch even if
   * the pointer we provided get deleted mid-way through the program lifetime
   */
  DRAudioStream *self = static_cast<DRAudioStream *>(userdata);
  for (auto &v : self->m_hsync_stack)
  {
    if (v.sync != hsync)
      continue;

    switch (v.type)
    {
    case BASS_SYNC_END:
      Q_EMIT self->finished();
      break;

    case BASS_SYNC_DEV_FAIL:
      Q_EMIT self->device_error(QPrivateSignal());
      break;
    }
  }
}

void DRAudioStream::cache_position()
{
  if (!m_hstream.has_value())
    return;
  if (m_position.has_value())
    return;
  m_position = BASS_ChannelGetPosition(m_hstream.value(), BASS_POS_BYTE);
  BASS_ChannelStop(m_hstream.value());
}

void DRAudioStream::on_device_error()
{
  cache_position();
  DRAudioEngine::check_stream_error();
}

void DRAudioStream::update_device()
{
  if (!m_hstream.has_value())
  {
    Q_EMIT finished();
    return;
  }

  if (is_playing())
    return;
  const QString file = m_file.value();

  m_file.reset();
  m_hstream.reset();
  m_hsync_stack.clear();

  blockSignals(true);
  set_file(file);
  set_volume(m_volume);
  blockSignals(false);

  if (m_position.has_value())
  {
    if (BASS_ChannelSetPosition(m_hstream.value(), m_position.value(), BASS_POS_BYTE) == FALSE)
      qWarning() << DRAudioError(
                        QString("failed to set position for %1: %2").arg(file).arg(DRAudio::get_last_bass_error()))
                        .what();
    m_position.reset();
  }

  play();
}
