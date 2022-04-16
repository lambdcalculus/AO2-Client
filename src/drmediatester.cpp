#include "drmediatester.h"

#include <QUrl>

#include "debug_functions.h"

DRMediaTester::DRMediaTester(QObject *parent) : QObject(parent)
{
  m_player.setMuted(true);
  connect(&m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this,
          SLOT(p_check_status(QMediaPlayer::MediaStatus)));
  m_player.setMedia(QUrl("qrc:/res/tests/mp4_media_test.mp4"));
}

DRMediaTester::~DRMediaTester()
{}

void DRMediaTester::p_check_status(QMediaPlayer::MediaStatus p_status)
{
  switch (p_status)
  {
  case QMediaPlayer::InvalidMedia:
    call_warning("Your operating system appears to not currently support video playback of <b>MP4</b> files. The video "
                 "playback will not work properly.<br /><br />In order for the feature to function properly, you "
                 "will need to install the appropriate codecs.<br /><br />For more information visit "
                 "<a href=\"https://www.danganronpaonline.com\">https://www.danganronpaonline.com</a>");
    break;

  default:
    break;
  }
}
