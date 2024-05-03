#ifndef DROANIMATION_H
#define DROANIMATION_H
#include <QElapsedTimer>
#include <datatypes.h>

class DROAnimation
{
public:
  DROAnimation();
  void RunAnimation();
  void SetNextKeyframe(AnimationVariableTypes animType);

  void Start(bool loop);
  void Stop();
  void Pause();
  bool getIsPlaying();
  void AddKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout);
  void setKeyframes(QVector<DROAnimationKeyframe> t_frames);

  float getValue(AnimationVariableTypes type);

  int getCurrentFrame();

private:
  qint64 mElapsedTime = 0;
  int mCurrentKeyframe = 0;
  bool mCurrentlyPlaying = false;
  bool isLoop = true;
  QElapsedTimer timer = QElapsedTimer();

  QHash<AnimationVariableTypes, int> mVarNextKeyframe = {};
  QHash<AnimationVariableTypes, int> mVarLastKeyframe = {};
  QHash<AnimationVariableTypes, float> mVarValues = {};

  QVector<DROAnimationKeyframe> keyFrames = {};
};

#endif // DROANIMATION_H
