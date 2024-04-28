#include "animation_manager.h"
#include "modules/theme/thememanager.h"

#include <QListWidget>



AnimationManager AnimationManager::s_Instance;

void AnimationManager::loadCharacterAnimations()
{
  mCharaAnimationNames.clear();
  mCharaAnimationNames.append("Default");

  mCharacterAnimations = {};
  QListWidget * l_listWidget = ThemeManager::get().getWidgetType<QListWidget>("chara_animations");

  mCharaAnimationNames.append("shake");
  mCharacterAnimations["shake"] = new AnimationReader(eAnimationPlayer, "shake");

  mCharaAnimationNames.append("popdown");
  mCharacterAnimations["popdown"] = new AnimationReader(eAnimationPlayer, "popdown");

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
