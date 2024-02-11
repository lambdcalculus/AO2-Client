#ifndef DRPLAYER_H
#define DRPLAYER_H

#include <qstring.h>



class DrPlayer
{
public:
    DrPlayer(int id, QString showname, QString character, QString url, QString status);

    void setMod(QString ipid, QString hdid);

    int m_id;
    QString m_showname;
    QString m_character;
    QString mURL;
    QString mPlayerStatus;
    QString mHDID;
    QString mIPID;
};

#endif // DRPLAYER_H
