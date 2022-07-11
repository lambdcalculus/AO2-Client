#include "drvideoscreen.h"

#include "aoapplication.h"
#include "aoconfig.h"
#include "draudiodevice.h"
#include "draudioengine.h"
#include "draudiostreamfamily.h"

#include <QAudioDeviceInfo>
#include <QAudioOutputSelectorControl>
#include <QMediaService>
#include <QUrl>

DRVideoWidget::DRVideoWidget(QWidget *parent)
    : QVideoWidget(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{
  Q_ASSERT(ao_app);

  m_config = new AOConfig(this);
  m_engine = new DRAudioEngine(this);
  m_family = m_engine->get_family(DRAudio::Family::FVideo);
  m_player = new QMediaPlayer(this);
  m_player->setVideoOutput(this);

  connect(m_engine, SIGNAL(current_device_changed(DRAudioDevice)), this, SLOT(update_device(DRAudioDevice)));
  connect(m_config, SIGNAL(video_volume_changed(int)), this, SLOT(update_volume()));
  connect(m_engine, SIGNAL(volume_changed(int32_t)), this, SLOT(update_volume()));
  connect(m_engine, SIGNAL(options_changed(DRAudio::Options)), this, SLOT(update_volume()));
  connect(m_family.get(), SIGNAL(volume_changed(int32_t)), this, SLOT(update_volume()));
  connect(m_family.get(), SIGNAL(options_changed(DRAudio::Options)), this, SLOT(update_volume()));
  connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this,
          SLOT(check_media_status(QMediaPlayer::MediaStatus)));
  connect(m_player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(check_video_availability(bool)));
  connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(check_state(QMediaPlayer::State)));

  const std::optional<DRAudioDevice> l_device = m_engine->get_current_device();
  update_device(l_device.value_or(m_device));
}

DRVideoWidget::~DRVideoWidget()
{}

QString DRVideoWidget::file_name()
{
  return m_file_name;
}

void DRVideoWidget::set_file_name(QString p_file_name)
{
  if (m_file_name == p_file_name)
    return;
  m_file_name = p_file_name;
  m_scanned = false;
  m_readable = false;
  m_running = false;
  stop();
  if (m_file_name.isEmpty())
  {
    m_scanned = true;
    return;
  }
  m_player->setMuted(true);
  m_player->setMedia(QUrl(p_file_name));
  if (m_player->error())
  {
    qDebug() << "error: failed to load media file" << (m_file_name.isEmpty() ? "<undefined>" : m_file_name);
  }
}

/**
 * @brief Attempts to play a video file located in a character's videos directory.
 *
 * If the file isn't valid for any reason (not found, error, file has no video track, etc) the signal done() will be
 * emitted.
 *
 * If a video file is already playing, it will immediately stop it
 * without emitting done() before playing the newly designated file
 */
void DRVideoWidget::play_character_video(QString p_character, QString p_video)
{
  qInfo() << "loading character media file" << p_character << p_video;

  QStringList l_filelist;
  const QString l_video_path = QString("videos/%1").arg(p_video);
  for (const QString &i_character_name : ao_app->get_char_include_tree(p_character))
  {
    l_filelist.append(ao_app->get_character_path(i_character_name, l_video_path));
  }

  const QString l_file = ao_app->find_asset_path(l_filelist);
  set_file_name(l_file);
  play();
}

void DRVideoWidget::play()
{
  if (!m_scanned)
  {
    m_running = true;
    return;
  }
  else if (!m_readable)
  {
    emit finished();
    return;
  }

  stop();
  m_running = true;
  m_player->setMuted(false);
  m_player->blockSignals(true); // prevents scan results from being altered
  m_player->setMedia(QUrl(m_file_name));
  m_player->blockSignals(false);
  m_player->play();
}

void DRVideoWidget::stop()
{
  m_running = false;
  m_player->blockSignals(true);
  m_player->stop();
  m_player->setMedia(nullptr);
  m_player->blockSignals(false);
}

bool DRVideoWidget::is_playable()
{
  return m_readable;
}

void DRVideoWidget::check_media_status(QMediaPlayer::MediaStatus p_status)
{
  if (p_status == QMediaPlayer::InvalidMedia)
  {
    qWarning() << "error: invalid media file" << m_file_name;
    m_scanned = true;
    handle_scan_error();
  }
}

void DRVideoWidget::check_video_availability(bool p_state)
{
  m_scanned = true;
  m_readable = p_state;
  m_player->setMuted(!m_readable);
  if (!m_readable)
  {
    qWarning() << "error: media file is not a video" << m_file_name;
    handle_scan_error();
  }
  else if (m_running && m_player->state() != QMediaPlayer::PlayingState)
  {
    m_player->play();
  }
}

void DRVideoWidget::check_state(QMediaPlayer::State p_state)
{
  if (p_state == QMediaPlayer::PlayingState)
  {
    update_audio_output();
    emit started();
  }
  else if (m_readable && m_running && p_state == QMediaPlayer::StoppedState)
  {
    qInfo() << "finished media file playback" << m_file_name;
    stop();
    emit finished();
  }
}

void DRVideoWidget::handle_scan_error()
{
  if (m_running)
  {
    stop();
    emit finished();
  }
}

void DRVideoWidget::update_device(DRAudioDevice p_device)
{
  if (m_device == p_device)
    return;
  m_device = p_device;
  update_audio_output();
}

void DRVideoWidget::update_audio_output()
{
  const QString l_new_device_name = m_device.get_name();
  QMediaService *l_service = m_player->service();
  QAudioOutputSelectorControl *l_control = l_service->requestControl<QAudioOutputSelectorControl *>();
  if (!l_control)
  {
    qWarning() << "error: missing audio output control, device unchanged";
    return;
  }

  const QStringList l_device_id_list = l_control->availableOutputs();
  for (const QString &i_device_id : l_device_id_list)
  {
    if (l_control->outputDescription(i_device_id) == l_new_device_name)
    {
      qDebug() << "Media player changed audio device;" << l_new_device_name;
      l_control->setActiveOutput(i_device_id);
      break;
    }
  }
  l_service->releaseControl(l_control);
}

void DRVideoWidget::update_volume()
{
  int l_volume = (m_family->get_volume() * m_engine->get_volume()) / 100;

  if (!m_family->is_ignore_suppression() && (m_family->is_suppressed() || m_engine->is_suppressed()))
  {
    l_volume = 0;
  }

  if (m_player->volume() == l_volume)
    return;
  m_player->setVolume(l_volume);
}
