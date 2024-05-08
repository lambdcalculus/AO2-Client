#ifndef REPLAYMANAGER_H
#define REPLAYMANAGER_H

#include <QElapsedTimer>
#include <QTimer>
#include <datatypes.h>

class ReplayManager
{
public:
  ReplayManager(const ReplayManager&) = delete;

  static ReplayManager& get()
  {
    return s_Instance;
  }

  void startRecording();
  void recordMusicOP(QString t_music);
  void recordArea(QString t_bgn);
  void recordMessage(QStringList t_message);

  void saveReplay();


private:
  ReplayManager() {}
  QElapsedTimer mRecorder;
  static ReplayManager s_Instance;

  QString mReplayFilePath = "replays/replay.json";

  QVector<ReplayOperation> mCurrentHubReplay = {};

};

#endif // REPLAYMANAGER_H
