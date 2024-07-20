#include "scenario_manager.h"

ScenarioManager ScenarioManager::s_Instance;

QStringList ScenarioManager::ParseMusicList(QStringList l_musicList)
{
  m_CurrentMusicList.clear();
  m_MusicCategories.clear();
  QStringList m_ReturnValue = {};

  QString m_CurrentCategory = "-- No Category --";

  bool m_CategoryNext = false;
  for(QString r_MusicTrack : l_musicList)
  {
    if(r_MusicTrack == "category")
    {
      m_CategoryNext = true;
      continue;
    }

    if(m_CategoryNext)
    {
      m_CurrentCategory = r_MusicTrack;
      m_CategoryNext = false;
      m_MusicCategories.append(r_MusicTrack);
      m_ReturnValue.append(r_MusicTrack);
      continue;
    }

    m_CurrentMusicList[m_CurrentCategory].append(r_MusicTrack);
    m_ReturnValue.append(r_MusicTrack);

  }
  return m_ReturnValue;
}
