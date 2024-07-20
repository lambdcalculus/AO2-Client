#ifndef SCENARIO_MANAGER_H
#define SCENARIO_MANAGER_H

#include <QString>
#include <datatypes.h>


class ICMessageData
{
public:
  ICMessageData(QStringList t_messageData, bool t_legacy);;
  QStringList LegacyPacketContents();
  QStringList PacketContents();

  bool m_DeskModifier = true;
  bool m_IsFlipped = false;
  bool m_HideCharacter = true;

  bool m_UsesPreAnimation = false;

  bool m_PairIsFlipped = false;
  bool m_PairIsHidden = false;

  QString m_PreAnimation = "";
  QString m_CharacterFolder = "";
  QString m_CharacterOutfit = "";
  QString m_CharacterEmotion = "";

  QString m_MessageContents = "";
  QString m_AreaPosition = "wit";
  QString m_SFXName = "";

  QString m_ShowName = "";
  QString m_VideoName = "";
  GameEffectData m_EffectData = GameEffectData("");

  QString m_PairCharacterFolder = "";
  QString m_PairCharacterEmotion = "";

  QString m_KeyframeAnimation = "";

  int m_EmoteModifier = 0; //CMEmoteModifier - Obsolete?
  int m_CharacterServerId = 0;
  int m_SFXDelay = 0;
  int m_ShoutModifier = 0;
  QString m_ShoutName = "";
  int m_EvidenceId = 0;


  int m_EffectState = 0;
  int m_TextColor = 0;
  int m_ClientId = 0;
  int m_OffsetX = 0;


  int m_PairOffsetX = 0;
  ChatTypes m_ChatType = ChatTypes::Talk;

};

class ScenarioManager
{
public:

  static ScenarioManager& get()
  {
    return s_Instance;
  }

  QStringList ParseMusicList(QStringList l_musicList);

  QStringList GetAllMusic()
  {
    QStringList l_returnValue = {};

    for(QString rMusicCategory : m_MusicCategories)
    {
      l_returnValue.append(rMusicCategory);
      for(QString r_MusicTrack : m_CurrentMusicList[rMusicCategory])
      {
        l_returnValue.append(r_MusicTrack);
      }
    }
    return l_returnValue;
  };

  QStringList GetCategoryMusic(QString t_category)
  {
    if(m_CurrentMusicList.contains(t_category))
    {
      return m_CurrentMusicList[t_category];
    }
    return {};
  };

  QStringList GetMusicCategories()
  {
    return m_MusicCategories;
  };

  QStringList GetPinnedTracks()
  {
    return m_PinnedTracks;
  };

  void PinTrack(QString t_track)
  {
    if(!m_PinnedTracks.contains(t_track)) m_PinnedTracks.append(t_track);
  };

private:
  ScenarioManager() {}
  static ScenarioManager s_Instance;

  QMap<QString, QStringList> m_CurrentMusicList = {};
  QStringList m_MusicCategories = {};
  QStringList m_PinnedTracks = {};

};

#endif // SCENARIO_MANAGER_H
