#ifndef DROVIEWPORTWIDGET_H
#define DROVIEWPORTWIDGET_H

#include <DRCharacterMovie.h>
#include <DREffectMovie.h>
#include <DRSceneMovie.h>
#include <DRSplashMovie.h>
#include <QObject>
#include <QWidget>
#include <drgraphicscene.h>
#include <drshoutmovie.h>

#include <mk2/graphicsvideoscreen.h>

#include <modules/managers/scene_manager.h>

class DROViewportWidget : public DRGraphicsView
{
  Q_OBJECT
public:
  explicit DROViewportWidget(QWidget *parent = nullptr);
  void ConstructViewport(ThemeSceneType t_scene);
  void ProcessIncomingMessage(ICMessageData *t_IncomingMessage);

  //Visual Updates
  void UpdateBackground(QString t_position = "wit");

signals:
  void VideoDone();
  void PreanimDone();

public slots:
  void OnVideoDone();
  void OnPreanimDone();

private:
  AOApplication *m_AOApp = nullptr;

  //Data
  ICMessageData *m_IncomingMessage = nullptr;

  //Graphics Items
  DRSceneMovie *m_VpBackground = nullptr;
  DRCharacterMovie *m_VpPlayer = nullptr;

  //Animations
  DREffectMovie *m_VpEffect = nullptr;
  DRSplashMovie *m_VpWtce = nullptr;
  DRShoutMovie *m_VpShouts = nullptr;
  DRVideoScreen *m_VpVideo = nullptr;

};

#endif // DROVIEWPORTWIDGET_H
