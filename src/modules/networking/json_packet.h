#ifndef JSONPACKET_H
#define JSONPACKET_H

#include <qstring.h>

class JsonPacket
{
public:
  JsonPacket();
  static void ProcessJson(QString p_jsonString);
};

#endif // JSONPACKET_H
