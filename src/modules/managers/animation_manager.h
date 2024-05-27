#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H
#include <QThread>
#include <QObject>

#include "modules/json/animation_reader.h"

#include <modules/theme/graphicobjectanimator.h>

#include <modules/widgets/keyframe_player.h>

class AnimationManager : public QObject
{
  Q_OBJECT
public:
  AnimationManager(const AnimationManager&) = delete;

  static AnimationManager& get()
  {
    return s_Instance;
  }

  void loadCharacterAnimations();
  QVector<DROAnimationKeyframe> getCharacterFrames(QString t_name);
  bool getCharacterLoop(QString t_name);

  void setScene(KeyframePlayer * t_scene);
  void addToQueue(DROAnimation * t_animation);

private:
  QThread *ThAnimationQueue = nullptr;

  QMap<QString, AnimationReader*> mCharacterAnimations = {};
  QStringList mCharaAnimationNames = {};
  KeyframePlayer *mShoutsScene = nullptr;
  QVector<DROAnimation *> mGraphicsRenderQueue = {};

private:
  AnimationManager()
  {

  }
  static AnimationManager s_Instance;

};

#endif // ANIMATIONMANAGER_H
