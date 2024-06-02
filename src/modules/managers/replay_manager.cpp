#include "game_manager.h"
#include "replay_manager.h"
#include "modules/managers/pathing_manager.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <aoapplication.h>
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

void ReplayManager::startRecording()
{
  QString mFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd (hh.mm.ss.z)'.json'");
  mReplayFilePath =  AOApplication::getInstance()->get_base_path() + "replays/" + mFileName;
  mRecorder.start();
}

void ReplayManager::recordMusicOP(QString t_music)
{
  ReplayOperation lNewOperation = ReplayOperation("bgm");
  lNewOperation.mTimestamp = mRecorder.elapsed();
  lNewOperation.mVariables["track"] = t_music;
  mCurrentHubReplay.append(lNewOperation);
  saveReplay();
}

void ReplayManager::recordArea(QString t_bgn)
{
  ReplayOperation lNewOperation = ReplayOperation("bg");
  lNewOperation.mTimestamp = mRecorder.elapsed();
  lNewOperation.mVariables["name"] = t_bgn;
  mCurrentHubReplay.append(lNewOperation);
  saveReplay();
}

void ReplayManager::recordMessage(QStringList t_message)
{
  ReplayOperation lNewOperation = ReplayOperation("msg");

  lNewOperation.mTimestamp = mRecorder.elapsed();

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
  mCurrentHubReplay.append(lNewOperation);
  saveReplay();
}

void ReplayManager::saveReplay()
{
  QJsonObject lReplayJson;

  QJsonArray lReplayOperations;

  for(ReplayOperation rOperation : mCurrentHubReplay)
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

  QFile file(mReplayFilePath);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    qDebug() << "Failed to open file for writing.";
    return;
  }

  QTextStream out(&file);
  out << lOutputJson.toJson();

  file.close();
}

void ReplayManager::loadReplayPlayback(QString t_name, ReplayScene *p_scene)
{
  mCurrentPlaybackIndex = 0;
  mReplayScene = p_scene;
  ReplayReader l_reader = ReplayReader(t_name);
  mPlaybackReplay = l_reader.getOperations();
  return;
}

QString ReplayManager::getPlaybackBackground()
{
  return "FTOD/Island 5/SurfaceCaveView";
}

void ReplayManager::progressPlayback()
{
  if(mPlaybackReplay.count() > (mCurrentPlaybackIndex + 1))
  {
    mCurrentPlaybackIndex += 1;
    QString mOp = mPlaybackReplay[mCurrentPlaybackIndex].mOperation;

    if(mOp == "msg")
    {
      mReplayScene->setBgPosition(mPlaybackReplay[mCurrentPlaybackIndex].mVariables["pos"]);
      mReplayScene->setMsgOperation(mPlaybackReplay[mCurrentPlaybackIndex].mVariables);
    }

    if(mOp == "bg")
    {
      mReplayScene->setBackground(mPlaybackReplay[mCurrentPlaybackIndex].mVariables["name"]);
    }

    if(mOp == "bgm")
    {
      mReplayScene->playSong(mPlaybackReplay[mCurrentPlaybackIndex].mVariables["track"]);
    }

    if(mOp != "msg") progressPlayback();
  }

}

void ReplayManager::clearPackagesReplays()
{
  mPackageNames.clear();
  mPackageReplays.clear();
  mPackageNames.append("Local Recordings");
}

void ReplayManager::cachePackageReplays(QString t_package, QVector<QString> t_tags)
{
  if(!mPackageNames.contains(t_package))
  {
    mPackageNames.append(t_package);
    mPackageReplays[t_package] = t_tags;
  }
}

QStringList ReplayManager::getPackageNames()
{
  return mPackageNames.toList();
}

QVector<QString> ReplayManager::getPackageCategoryList(QString t_package)
{
  return mPackageReplays[t_package];
}
