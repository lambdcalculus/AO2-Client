#include "animation_manager.h"
#include "pathing_manager.h"
#include "modules/theme/thememanager.h"
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QListWidget>



AnimationManager AnimationManager::s_Instance;

void AnimationManager::loadCharacterAnimations()
{
  mCharaAnimationNames.clear();
  mCharaAnimationNames.append("Default");

  mCharacterAnimations = {};
  QListWidget * l_listWidget = ThemeManager::get().getWidgetType<QListWidget>("chara_animations");
  QStringList lCharacterAnims = PathingManager::get().searchDirectoryContentsFirst("animations/characters/", "json", false);

  for(QString rAnimName : lCharacterAnims)
  {
    mCharaAnimationNames.append(rAnimName);
    mCharacterAnimations[rAnimName] = new AnimationReader(eAnimationPlayer, rAnimName);
  }

  if(l_listWidget != nullptr)
  {
    l_listWidget->clear();
    l_listWidget->addItems(mCharaAnimationNames);
  }
}

QVector<DROAnimationKeyframe> AnimationManager::getCharacterFrames(QString t_name)
{
  if(mCharacterAnimations.contains(t_name)) return mCharacterAnimations[t_name]->getFrames("player");
  return {};
}

bool AnimationManager::getCharacterLoop(QString t_name)
{
  if(mCharacterAnimations.contains(t_name)) return mCharacterAnimations[t_name]->getCanLoop();
  return true;
}

void AnimationManager::setScene(KeyframePlayer *t_scene)
{
  mShoutsScene = t_scene;
}

void AnimationManager::addToQueue(DROAnimation *t_animation)
{
  mGraphicsRenderQueue.append(t_animation);
}

void AnimationManager::startQueue()
{
  //QtConcurrent::run(this, &AnimationManager::loopRenderQueue);

  //ThAnimationQueue->start();
}

void AnimationManager::loopRenderQueue()
{
  while(true)
  {
    if(mGraphicsRenderQueue.count() == 0) continue;
    for(DROAnimation * animation : mGraphicsRenderQueue)
    {
      if(animation->getIsPlaying()) animation->RunAnimation();
    }
  }
}
