#pragma once

#include <QMediaPlayer>
#include <QObject>

class DRMediaTester : public QObject
{
  Q_OBJECT

public:
  DRMediaTester(QObject *parent = nullptr);
  ~DRMediaTester();

signals:
  void done();

private:
  QMediaPlayer m_player;

private slots:
  void p_check_status(QMediaPlayer::MediaStatus p_status);
};
