#pragma once

#include <QMediaPlayer>
#include <QVideoWidget>

#include "draudiodevice.h"
#include "draudiostreamfamily.h"

class AOApplication;
class AOConfig;
class DRAudioEngine;

class DRVideoWidget : public QVideoWidget
{
  Q_OBJECT

public:
  DRVideoWidget(QWidget *parent = nullptr);
  ~DRVideoWidget();

  QString file_name();
  void set_file_name(QString file_name);

  void play_character_video(QString character, QString video);
  void play();
  void stop();

  bool is_playable();

signals:
  void done();

private:
  AOConfig *m_config;
  AOApplication *ao_app;
  DRAudioEngine *m_engine;
  DRAudioStreamFamily::ptr m_family;

  QString m_file_name;
  bool m_scanned = true;
  bool m_readable = false;
  QMediaPlayer *m_player;
  bool m_running = false;

  void handle_scan_error();

private slots:
  void update_device(DRAudioDevice);
  void update_volume();
  void check_media_status(QMediaPlayer::MediaStatus);
  void check_video_availability(bool);
  void check_state(QMediaPlayer::State);
};
