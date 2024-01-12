#include "json_packet.h"
#include "qjsonobject.h"
#include "aoapplication.h"
#include "courtroom.h"
#include "modules/json/json_reader.h"

#include <qjsondocument.h>

JsonPacket::JsonPacket()
{

}

void JsonPacket::ProcessJson(QString p_jsonString)
{
  JSONReader jsonReader = JSONReader();
  jsonReader.ReadFromString(p_jsonString);

  QString packetValue = jsonReader.getStringValue("packet");

  if(packetValue == "player_list")
  {
    QJsonArray playerArray = jsonReader.getArrayValue("data");
    SceneManager::get().clearPlayerDataList();
    for(QJsonValueRef ref : playerArray)
    {
      jsonReader.SetTargetObject(ref.toObject());
      int playerId = jsonReader.getStringValue("id").toInt();
      QString showname = jsonReader.getStringValue("showname");
      QString characterName = jsonReader.getStringValue("character");
      QString charaURL = jsonReader.getStringValue("url");
      QString statusPlayer = jsonReader.getStringValue("status");

      DrPlayer* drp = new DrPlayer(playerId, showname, characterName, charaURL, statusPlayer);
      SceneManager::get().mPlayerDataList.append(*drp);
    }
    AOApplication::getInstance()->m_courtroom->construct_playerlist_layout();
  }
}
