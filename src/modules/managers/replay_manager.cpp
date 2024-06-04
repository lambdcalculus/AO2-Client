#include "game_manager.h"
#include "replay_manager.h"
#include "modules/managers/pathing_manager.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <AOApplication.h>
#include <QDir>
#include <modules/json/replay_reader.h>

ReplayManager ReplayManager::s_Instance;

QStringList ReplayManager::getReplayList(QString t_package, QString t_category)
{
  QString t_path = "replays/";
  if(!t_category.trimmed().isEmpty()) t_path += (t_category + "/");
  return PathingManager::get().searchDirectoryContentsSpecific(t_path, "json", t_package, false);
}

QString ReplayManager::getReplayPath(QString t_package, QString t_category, QString t_name)
{
  QString t_path = "";
  if(t_package.trimmed().isEmpty())
  {
    t_path = PathingManager::get().getBasePath() + "replays/";
  }
  else
  {
    t_path = PathingManager::get().getPackagePath(t_package) + "replays/";
  }

  if(!t_category.trimmed().isEmpty()) t_path += (t_category + "/");
  t_path += t_name;
  t_path += ".json";

  return t_path;
}

QString ReplayManager::getReplayImagePath(QString t_package, QString t_category, QString t_name)
{
  QString t_path = "";
  if(t_package.trimmed().isEmpty())
  {
    t_path = PathingManager::get().getBasePath() + "replays/";
  }
  else
  {
    t_path = PathingManager::get().getPackagePath(t_package) + "replays/";
  }

  if(!t_category.trimmed().isEmpty()) t_path += (t_category + "/");
  t_path += t_name;
  t_path += ".png";

  return t_path;
}

void ReplayManager::RecordingStart()
{
  QString l_FileName = QDateTime::currentDateTime().toString("yyyy-MM-dd (hh.mm.ss.z)'.json'");
  m_FilePathOutput =  AOApplication::getInstance()->get_base_path() + "replays/" + l_FileName;
  m_TimerRecorder.start();
}

void ReplayManager::RecordChangeMusic(QString t_music)
{
  ReplayOperation lNewOperation = ReplayOperation("bgm");
  lNewOperation.mTimestamp = m_TimerRecorder.elapsed();
  lNewOperation.mVariables["track"] = t_music;
  m_ReplayOperationsRecorded.append(lNewOperation);
  RecordingSave();
}

void ReplayManager::RecordChangeBackground(QString t_bgn)
{
  ReplayOperation lNewOperation = ReplayOperation("bg");
  lNewOperation.mTimestamp = m_TimerRecorder.elapsed();
  lNewOperation.mVariables["name"] = t_bgn;
  m_ReplayOperationsRecorded.append(lNewOperation);
  RecordingSave();
}

void ReplayManager::RecordMessageIC(QStringList t_message)
{
  ReplayOperation lNewOperation = ReplayOperation("msg");

  lNewOperation.mTimestamp = m_TimerRecorder.elapsed();

  GameEffectData lEffect = GameManager::get().getEffect(t_message[CMEffectState].toInt());

  lNewOperation.mVariables["pre"] = t_message[CMPreAnim];
  lNewOperation.mVariables["char"] = t_message[CMChrName];
  lNewOperation.mVariables["emote"] = t_message[CMEmote];
  lNewOperation.mVariables["msg"] = t_message[CMMessage];
  lNewOperation.mVariables["pos"] = t_message[CMPosition];
  lNewOperation.mVariables["sound"] = t_message[CMSoundName];
  lNewOperation.mVariables["color"] = t_message[CMTextColor];
  lNewOperation.mVariables["showname"] = t_message[CMShowName];
  lNewOperation.mVariables["video"] = t_message[CMVideoName];
  lNewOperation.mVariables["hide"] = t_message[CMHideCharacter];
  lNewOperation.mVariables["flip"] = t_message[CMFlipState];
  lNewOperation.mVariables["effect"] = lEffect.mName;
  lNewOperation.mVariables["shout"] = t_message[CMShoutModifier];
  m_ReplayOperationsRecorded.append(lNewOperation);
  RecordingSave();
}

void ReplayManager::RecordMessageOOC(QString t_message)
{
  //TO-DO: Implement
}

void ReplayManager::RecordChangeWeather(QString t_weather)
{
  //TO-DO: Implement
}

void ReplayManager::RecordChangeGamemode(QString t_gamemode)
{
  //TO-DO: Implement
}

void ReplayManager::RecordChangeHour(int t_hour)
{
  //TO-DO: Implement
}

void ReplayManager::RecordChangeTOD(QString t_tod)
{
  //TO-DO: Implement
}

void ReplayManager::RecordingSave()
{
  QJsonObject lReplayJson;

  QJsonArray lReplayOperations;

  for(ReplayOperation rOperation : m_ReplayOperationsRecorded)
  {
    QJsonObject rNewOperations;
    rNewOperations["op"] = rOperation.mOperation;
    rNewOperations["time"] = rOperation.mTimestamp;

    QMap<QString, QString>::const_iterator i;
    for (i = rOperation.mVariables.constBegin(); i != rOperation.mVariables.constEnd(); ++i)
    {
      rNewOperations[i.key()] = i.value();
    }

    lReplayOperations.append(rNewOperations);
  }

  lReplayJson["script"] = lReplayOperations;


  QJsonDocument lOutputJson(lReplayJson);

  QFile file(m_FilePathOutput);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    qDebug() << "Failed to open file for writing.";
    return;
  }

  QTextStream out(&file);
  out << lOutputJson.toJson();

  file.close();
}

void ReplayManager::PlaybackLoadFile(QString t_name, ReplayScene *p_scene)
{
  m_PlaybackPositionIndex = 0;
  p_SceneReplay = p_scene;
  ReplayReader l_reader = ReplayReader(t_name);
  m_ReplayOperationsPlayback = l_reader.getOperations();
  return;
}

void ReplayManager::PlaybackProgressManual()
{
  if(m_ReplayOperationsPlayback.count() > (m_PlaybackPositionIndex + 1))
  {
    m_PlaybackPositionIndex += 1;
    QString mOp = m_ReplayOperationsPlayback[m_PlaybackPositionIndex].mOperation;

    if(mOp == "msg")
    {
      p_SceneReplay->setBgPosition(m_ReplayOperationsPlayback[m_PlaybackPositionIndex].mVariables["pos"]);
      p_SceneReplay->setMsgOperation(m_ReplayOperationsPlayback[m_PlaybackPositionIndex].mVariables);
    }

    if(mOp == "bg")
    {
      p_SceneReplay->setBackground(m_ReplayOperationsPlayback[m_PlaybackPositionIndex].mVariables["name"]);
    }

    if(mOp == "bgm")
    {
      p_SceneReplay->playSong(m_ReplayOperationsPlayback[m_PlaybackPositionIndex].mVariables["track"]);
    }

    if(mOp != "msg") PlaybackProgressManual();
  }

}

void ReplayManager::ResetPackagesCache()
{
  m_PackagesContaining.clear();
  m_PackageCategories.clear();
  m_PackagesContaining.append("Local Recordings");
}

void ReplayManager::CachePackageReplays(QString t_package, QVector<QString> t_tags)
{
  if(!m_PackagesContaining.contains(t_package))
  {
    m_PackagesContaining.append(t_package);
    m_PackageCategories[t_package] = t_tags;
  }
}

QStringList ReplayManager::getPackageNames()
{
  return m_PackagesContaining.toList();
}

QVector<QString> ReplayManager::getPackageCategoryList(QString t_package)
{
  return m_PackageCategories[t_package];
}
