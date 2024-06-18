#ifndef DROANIMATION_H
#define DROANIMATION_H
#include <QElapsedTimer>
#include <datatypes.h>

class DROAnimation
{
public:
  DROAnimation();
  void RunAnimation();

  bool GetCurrentlyRunning();
  float GetCurrentValue(AnimationVariableTypes type);

  void SetNextKeyframe(AnimationVariableTypes animType);

  void Start(bool loop);
  void Stop();
  void Pause();

  void AddKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout);
  void SetKeyframes(QVector<DROAnimationKeyframe> t_frames);

  int GetCurrentFrame();

private:

  QElapsedTimer m_AnimationTimer = QElapsedTimer();
  qint64 m_TimeElapsed = 0;
  bool m_IsRunning = false;

  bool m_AnimationLoops = true;

  int m_KeyframeIndex = 0;
  QVector<DROAnimationKeyframe> m_AnimationKeyframes = {};

  QHash<AnimationVariableTypes, int> m_UpcomingValues = {};
  QHash<AnimationVariableTypes, int> m_PreviousValues = {};
  QHash<AnimationVariableTypes, float> m_CurrentValues = {};
};

#endif // DROANIMATION_H
