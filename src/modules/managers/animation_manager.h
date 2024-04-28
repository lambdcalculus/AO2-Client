#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "modules/json/animation_reader.h"

class AnimationManager
{
public:
  AnimationManager(const AnimationManager&) = delete;

  static AnimationManager& get()
  {
    return s_Instance;
  }

  void loadCharacterAnimations();
  QVector<DROAnimationKeyframe> getCharacterFrames(QString t_name);
  bool getCharacterLoop(QString t_name);

private:
  QMap<QString, AnimationReader*> mCharacterAnimations = {};
  QStringList mCharaAnimationNames = {};

private:
  AnimationManager()
  {

  }
  static AnimationManager s_Instance;

};

#endif // ANIMATIONMANAGER_H
