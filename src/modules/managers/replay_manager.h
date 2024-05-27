#ifndef REPLAYMANAGER_H
#define REPLAYMANAGER_H

#include <QElapsedTimer>
#include <QTimer>
#include <datatypes.h>

#include <modules/scenes/replay_scene.h>

class ReplayManager
{
public:
  ReplayManager(const ReplayManager&) = delete;

  static ReplayManager& get()
  {
    return s_Instance;
  }

  QStringList getReplayList();

  void loadReplay();

  void startRecording();
  void recordMusicOP(QString t_music);
  void recordArea(QString t_bgn);
  void recordMessage(QStringList t_message);

  void saveReplay();


  void loadReplayPlayback(QString t_name, ReplayScene *p_scene);
  QString getPlaybackBackground();
  void progressPlayback();

  void clearPackagesReplays();
  void cachePackageReplays(QString t_package, QVector<QString> t_tags);


private:
  QVector<QString> mPackageNames = {};
  QHash<QString, QVector<QString>> mPackageReplays = {};
  ReplayManager() {}
  QElapsedTimer mRecorder;
  static ReplayManager s_Instance;

  QString mReplayFilePath = "replays/replay.json";

  QVector<ReplayOperation> mCurrentHubReplay = {};


  ReplayScene *mReplayScene = nullptr;
  int mCurrentPlaybackIndex = 0;
  QVector<ReplayOperation> mPlaybackReplay = {};

};

#endif // REPLAYMANAGER_H
