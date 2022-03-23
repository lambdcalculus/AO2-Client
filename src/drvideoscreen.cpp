#include "drvideoscreen.h"

#include <QAudioDeviceInfo>
#include <QAudioOutputSelectorControl>
#include <QMediaService>
#include <QUrl>

#include "aoapplication.h"
#include "aoconfig.h"
#include "draudiodevice.h"
#include "draudioengine.h"
#include "draudiostreamfamily.h"

DRVideoWidget::DRVideoWidget(QWidget *parent) : QVideoWidget(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{
  Q_ASSERT(ao_app);

  m_config = new AOConfig(this);
  m_engine = new DRAudioEngine(this);
  m_family = m_engine->get_family(DRAudio::Family::FVideo);
  m_player = new QMediaPlayer(this);
  m_player->setVideoOutput(this);

  connect(m_engine, SIGNAL(device_changed(DRAudioDevice)), this, SLOT(update_device(DRAudioDevice)));
  connect(m_config, SIGNAL(video_volume_changed(int)), this, SLOT(update_volume()));
  connect(m_engine, SIGNAL(volume_changed(int32_t)), this, SLOT(update_volume()));
  connect(m_engine, SIGNAL(options_changed(DRAudio::Options)), this, SLOT(update_volume()));
  connect(m_family.get(), SIGNAL(volume_changed(int32_t)), this, SLOT(update_volume()));
  connect(m_family.get(), SIGNAL(options_changed(DRAudio::Options)), this, SLOT(update_volume()));
  connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this,
          SLOT(check_media_status(QMediaPlayer::MediaStatus)));
  connect(m_player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(check_video_availability(bool)));
  connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(check_state(QMediaPlayer::State)));
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
  if (p_file_name.isEmpty())
    m_scanned = true;
  m_readable = false;
  m_running = false;
  stop();
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
    emit done();
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
  if (m_readable && m_running && p_state == QMediaPlayer::StoppedState)
  {
    qInfo() << "finished media file playback" << m_file_name;
    stop();
    emit done();
  }
}

void DRVideoWidget::handle_scan_error()
{
  if (m_running)
  {
    stop();
    emit done();
  }
}

void DRVideoWidget::update_device(DRAudioDevice p_device)
{
  const QString l_new_device_name = p_device.get_name();
  const QList<QAudioDeviceInfo> l_device_list = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
  for (const QAudioDeviceInfo &i_device : l_device_list)
  {
    const QString l_device_name = i_device.deviceName();
    if (l_device_name == l_new_device_name)
    {
      QMediaService *l_service = m_player->service();
      QAudioOutputSelectorControl *l_control = l_service->requestControl<QAudioOutputSelectorControl *>();
      if (!l_control)
      {
        qCritical() << "error: failed to change device; QAudioOutputSelectorControl is null";
        return;
      }
      l_control->setActiveOutput(i_device.deviceName());
      l_service->releaseControl(l_control);
      break;
    }
  }
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
