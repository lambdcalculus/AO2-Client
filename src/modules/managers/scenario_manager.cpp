#include "scenario_manager.h"

#include <drgraphicscene.h>
#include <qpixmap.h>

#include <modules/theme/thememanager.h>

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

void ScenarioManager::ScreenshotViewport()
{
  DRGraphicsView *l_viewport = ThemeManager::get().GetWidgetType<DRGraphicsView>("viewport");

  if(l_viewport != nullptr)
  {
    QPixmap l_Pixmap = l_viewport->grab();
    QString l_FileName = QDateTime::currentDateTime().toString("yyyy-MM-dd (hh.mm.ss.z)'.png'");
    QString l_Path = "base/screenshots/" + l_FileName;
    if (!l_Pixmap.save(l_Path, "PNG")) {
      qWarning("Failed to save the screenshot.");
    }
  }
}
