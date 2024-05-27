#ifndef REPLAYSCENE_H
#define REPLAYSCENE_H

#include <QObject>
#include <QWidget>
#include <drcharactermovie.h>
#include "drgraphicscene.h"
#include "drscenemovie.h"
#include <AOSfxPlayer.h>
#include <aoapplication.h>
#include <aomusicplayer.h>
#include <dreffectmovie.h>
#include <drshoutmovie.h>
#include <drsplashmovie.h>
#include <drtextedit.h>
#include "modules/widgets/typewritertextedit.h"
#include <mk2/graphicsvideoscreen.h>

class ReplayScene : public QWidget
{
  Q_OBJECT
public:
  ReplayScene(AOApplication *p_ao_app, QWidget *parent = nullptr);
  ~ReplayScene();


  void playSong(QString t_song);
  void setText(QString t_dialogue);
  void setMsgOperation(QMap<QString, QString> t_vars);
  void setCharacter(QString t_name, QString t_emote, QString t_pre);
  void setBackground(QString t_name);
  void setBgPosition(QString t_name);

public slots:
  void videoDone();
  void preanim_done();

private:
  AOApplication *pAOApp = nullptr;
  QMap<QString, QString> mMsgVariables = {};

  void constructWidgets();



protected:
  void keyPressEvent(QKeyEvent *event) override;

private: //Viewport
  DRSceneMovie *vpBackground = nullptr;
  DRCharacterMovie *vpPlayer = nullptr;

  //Animations
  DREffectMovie *vpEffect = nullptr;
  DRSplashMovie *vpWtce = nullptr;
  DRShoutMovie *vpObjection = nullptr;
  DRVideoScreen *vpVideo = nullptr;

  AOSfxPlayer *pSfxPlayer = nullptr;
  AOMusicPlayer *pMusicPlayer = nullptr;
  DRTextEdit *vpShowname = nullptr;
  TypewriterTextEdit *vpMessage = nullptr;
  DRTextEdit *vpMessageShowname = nullptr;


};

#endif // REPLAYSCENE_H
