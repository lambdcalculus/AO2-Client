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

  //Player Animation Management
  void CachePlayerAnimations();
  QVector<DROAnimationKeyframe> GetPlayerFrames(QString t_name);
  bool GetPlayerAnimLoops(QString t_name);

  QString getPlayerAnimSound(QString t_name)
  {
    if(m_PlayerAnimations.contains(t_name)) return m_PlayerAnimations[t_name]->getAudioName();
    return "";
  };

private:
  AnimationManager() { }
  static AnimationManager s_Instance;


  QMap<QString, AnimationReader*> m_PlayerAnimations = {};
  QStringList m_CachedPlayerAnimNames = {};
  QVector<DROAnimation *> mGraphicsRenderQueue = {};

};

#endif // ANIMATIONMANAGER_H
