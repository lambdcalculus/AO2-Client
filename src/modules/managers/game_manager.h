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
  GameEffectData getEffect(int t_id);

  //Setup
  void setupGame();


  //Server Stuff
  void setServerFunctions(QStringList tFunctionList);
  bool usesServerFunction(QString tFunctionName);


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

  //Game Loop Variables
  int m_FramesPerSecond = 60;
  bool m_IsUpdateRunning = false;

  //Game Timings
  QTimer m_FrameTimer;
  int m_GameUptime = 0;

  //Widgets
  TypewriterTextEdit *m_WidgetTypeWriter = nullptr;

  //Animations
  GraphicObjectAnimator *m_PlayerAnimation = nullptr;
  QMap<AnimTypes, QVector<GraphicObjectAnimator *>> m_GraphicObjectAnimations = {};

  //Data
  QVector<GameEffectData> m_GameEffects = {};
  QStringList m_ServerFeatures = {};

  void StartGameLoop();
  void setupGameEffects();
};

#endif // GAMEMANAGER_H
