#ifndef REPLAYMANAGER_H
#define REPLAYMANAGER_H

#include <datatypes.h>

class ReplayManager
{
public:
  ReplayManager(const ReplayManager&) = delete;

  static ReplayManager& get()
  {
    return s_Instance;
  }

  void recordMusicOP(QString t_music);
  void recordArea(QString t_bgn);
  void recordMessage(QStringList t_message);

  void saveReplay();


private:
  ReplayManager() {}
  static ReplayManager s_Instance;

  QVector<ReplayOperation> mCurrentHubReplay = {};

};

#endif // REPLAYMANAGER_H
