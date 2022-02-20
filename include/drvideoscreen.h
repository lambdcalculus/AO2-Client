#pragma once

#include <QMediaPlayer>
#include <QWidget>

class QVideoWidget;

#include "draudiodevice.h"
#include "draudiostreamfamily.h"

class AOApplication;
class AOConfig;
class DRAudioEngine;

class DRVideoWidget : public QWidget
{
  Q_OBJECT

public:
  DRVideoWidget(QWidget *parent = nullptr);
  ~DRVideoWidget();

  void play(QString character, QString video);
  void stop();

signals:
  void done();

protected:
  void resizeEvent(QResizeEvent *event) final;

private:
  AOConfig *m_config;
  AOApplication *ao_app;
  DRAudioEngine *m_engine;
  DRAudioStreamFamily::ptr m_family;

  QMediaPlayer *m_player;
  QVideoWidget *m_screen;

private slots:
  void update_volume();
  void on_state_changed(QMediaPlayer::State);
  void on_video_availability_changed(bool);
  void on_device_changed(DRAudioDevice);
};
