#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QWidget>
#include "datatypes.h"

#include <modules/theme/graphicobjectanimator.h>

class GameManager : public QObject
{
  Q_OBJECT
public:
  GameManager(const GameManager&) = delete;

  static GameManager& get()
  {
    return s_Instance;
  }

  void StartGameLoop();
  void StopGameLoop();

  void SetPlayerAnimation(GraphicObjectAnimator * t_animation);
  void SetAnimationGroup(AnimTypes t_type, QVector<GraphicObjectAnimator *> t_animations);
  void RunAnimationLoop(AnimTypes t_type);

public slots:
  void RunGameLoop();

signals:
  void FrameComplete();
  void JudgeComplete();
  void ShoutComplete();

private:
  GameManager() {}
  static GameManager s_Instance;

  //Frame Loop Variables
  int mFPS = 60;
  bool mFlgUpdateRunning = false;
  QTimer mFrameTimer;

  GraphicObjectAnimator *mPlayerAnimation = nullptr;

  QMap<AnimTypes, QVector<GraphicObjectAnimator *>> mRuntimeAnimation = {};

};

#endif // GAMEMANAGER_H
