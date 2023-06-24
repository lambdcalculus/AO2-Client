#ifndef DRPLAYER_H
#define DRPLAYER_H

#include <qstring.h>



class DrPlayer
{
public:
    DrPlayer(int id, QString showname, QString character);

    int m_id;
    QString m_showname;
    QString m_character;
};

#endif // DRPLAYER_H
