#include "drvideoscreen.h"

#include <QAudioDeviceInfo>
#include <QAudioOutputSelectorControl>
#include <QMediaService>
#include <QUrl>
#include <QVideoWidget>

#include "aoapplication.h"
#include "aoconfig.h"
#include "draudiodevice.h"
#include "draudioengine.h"
#include "draudiostreamfamily.h"

DRVideoWidget::DRVideoWidget(QWidget *parent) : QWidget(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{
  Q_ASSERT(ao_app);

  m_config = new AOConfig(this);
  m_engine = new DRAudioEngine(this);
  m_family = m_engine->get_family(DRAudio::Family::FVideo);
  m_player = new QMediaPlayer(this);
  m_screen = new QVideoWidget(this);
  m_player->setVideoOutput(m_screen);

  connect(m_config, SIGNAL(video_volume_changed(int)), this, SLOT(update_volume()));
  connect(m_engine, SIGNAL(volume_changed(int32_t)), this, SLOT(update_volume()));
  connect(m_engine, SIGNAL(options_changed(DRAudio::Options)), this, SLOT(update_volume()));
  connect(m_family.get(), SIGNAL(volume_changed(int32_t)), this, SLOT(update_volume()));
  connect(m_family.get(), SIGNAL(options_changed(DRAudio::Options)), this, SLOT(update_volume()));
  connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_state_changed(QMediaPlayer::State)));
  connect(m_player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(on_video_availability_changed(bool)));
}

DRVideoWidget::~DRVideoWidget()
{}

/**
 * @brief Attempts to play a video file located in a character's videos directory.
 *
 * If the file isn't valid for any reason (not found, error, file has no video track, etc) the signal done() will be
 * emitted.
 *
 * If a video file is already playing, it will immediately stop it
 * without emitting done() before playing the newly designated file
 */
void DRVideoWidget::play(QString p_character, QString p_video)
{
  m_player->stop();
  m_player->setMedia(nullptr);

  qInfo() << "DRVideoWidget::play"
          << "starting:"
          << "character:" << p_character << "video:" << p_video;

  QStringList l_filelist;
  const QString l_video_path = QString("videos/%1").arg(p_video);
  for (const QString &i_character_name : ao_app->get_char_include_tree(p_character))
  {
    l_filelist.append(ao_app->get_character_path(i_character_name, l_video_path));
  }

  const QString l_file = ao_app->find_asset_path(l_filelist);
  if (l_file.isEmpty())
  {
    qWarning() << "DRVideoWidget::play"
               << "error: file could not be found";
    emit done();
    return;
  }

  m_player->setMedia(QUrl(l_file));
  m_player->setMuted(true);
  m_player->play();
  if (m_player->error())
  {
    qWarning() << "DRVideoWidget::play"
               << "error:" << m_player->errorString();
    emit done();
  }
}

void DRVideoWidget::stop()
{
  m_player->stop();
}

void DRVideoWidget::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);
  m_screen->resize(size());
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

void DRVideoWidget::on_state_changed(QMediaPlayer::State p_state)
{
  switch (p_state)
  {
  case QMediaPlayer::StoppedState:
    emit done();
    break;

  default:
    break;
  }
}

void DRVideoWidget::on_video_availability_changed(bool p_state)
{
  m_player->setMuted(!p_state);
  if (!p_state)
  {
    qWarning() << "DRVideoWidget::play"
               << "error: video track not available";
    m_player->stop();
    emit done();
  }
}

void DRVideoWidget::on_device_changed(DRAudioDevice p_device)
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
        qCritical() << "DRVideoWidget"
                    << "error: failed to change device; QAudioOutputSelectorControl is null";
        return;
      }
      l_control->setActiveOutput(i_device.deviceName());
      l_service->releaseControl(l_control);
      break;
    }
  }
}
