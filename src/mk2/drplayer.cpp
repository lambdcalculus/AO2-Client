#include "drplayer.h"
#include "qdebug.h"

DrPlayer::DrPlayer(int id, QString showname, QString character, QString url, QString status)
{
    m_id = id;
    m_showname = showname;
    m_character = character;
    mURL = url;
    mPlayerStatus = status;
}

void DrPlayer::setMod(QString ipid, QString hdid)
{
  mHDID = hdid;
  mIPID = ipid;
}
