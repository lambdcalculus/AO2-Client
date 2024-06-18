#include "animation_manager.h"
#include "pathing_manager.h"
#include "modules/theme/thememanager.h"
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QListWidget>



AnimationManager AnimationManager::s_Instance;

void AnimationManager::CachePlayerAnimations()
{
  m_CachedPlayerAnimNames.clear();
  m_CachedPlayerAnimNames.append("Default");

  m_PlayerAnimations = {};
  QListWidget * l_listWidget = ThemeManager::get().GetWidgetType<QListWidget>("chara_animations");
  QStringList lCharacterAnims = PathingManager::get().searchDirectoryContentsFirst("animations/characters/", "json", false);

  for(QString rAnimName : lCharacterAnims)
  {
    m_CachedPlayerAnimNames.append(rAnimName);
    m_PlayerAnimations[rAnimName] = new AnimationReader(eAnimationPlayer, rAnimName);
  }

  if(l_listWidget != nullptr)
  {
    l_listWidget->clear();
    l_listWidget->addItems(m_CachedPlayerAnimNames);
  }
}

QVector<DROAnimationKeyframe> AnimationManager::GetPlayerFrames(QString t_name)
{
  //TO-DO: I want to allow character folders to have their own animation files that can be overwritten if needed, similar to how shouts work.
  if(m_PlayerAnimations.contains(t_name)) return m_PlayerAnimations[t_name]->getFrames("player");
  return {};
}

bool AnimationManager::GetPlayerAnimLoops(QString t_name)
{
  if(m_PlayerAnimations.contains(t_name)) return m_PlayerAnimations[t_name]->getCanLoop();
  return true;
}


