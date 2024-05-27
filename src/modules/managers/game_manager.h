#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QWidget>
#include "datatypes.h"

#include <modules/theme/graphicobjectanimator.h>
#include "modules/widgets/typewritertextedit.h"

class GameManager : public QObject
{
  Q_OBJECT
public:
  GameManager(const GameManager&) = delete;

  static GameManager& get()
  {
    return s_Instance;
  }

  void StopGameLoop();

  void SetPlayerAnimation(GraphicObjectAnimator * t_animation);
  void SetTypeWriter(TypewriterTextEdit * t_writer);
  void SetAnimationGroup(AnimTypes t_type, QVector<GraphicObjectAnimator *> t_animations);
  void RunAnimationLoop(AnimTypes t_type);

  //Data Gathering
  GameEffectData getEffect(QString t_name);

  //Setup
  void setupGame();



  int getUptime();

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

  int mUptime = 0;

  GraphicObjectAnimator *mPlayerAnimation = nullptr;

  TypewriterTextEdit *mMessageTypeWriter = nullptr;
  QMap<AnimTypes, QVector<GraphicObjectAnimator *>> mRuntimeAnimation = {};

  QVector<GameEffectData> mGameEffects = {};

  void StartGameLoop();
  void setupGameEffects();
};

#endif // GAMEMANAGER_H
