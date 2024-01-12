#ifndef DRPLAYER_H
#define DRPLAYER_H

#include <qstring.h>



class DrPlayer
{
public:
    DrPlayer(int id, QString showname, QString character, QString url, QString status);

    int m_id;
    QString m_showname;
    QString m_character;
    QString mURL;
    QString mPlayerStatus;
};

#endif // DRPLAYER_H
