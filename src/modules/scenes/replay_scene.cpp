#include "replay_scene.h"
#include "commondefs.h"
#include "modules/theme/thememanager.h"
#include "modules/managers/scene_manager.h"
#include "qevent.h"
#include "modules/managers/game_manager.h"

#include <modules/managers/replay_manager.h>

ReplayScene::ReplayScene(AOApplication *p_ao_app, QWidget *parent) : QWidget(parent)
{
  GameManager::get().StartGameLoop();
  pAOApp = p_ao_app;
  constructWidgets();
  setWindowTitle("Replay");
  setFocusPolicy(Qt::StrongFocus);
}

void ReplayScene::setText(QString t_dialogue)
{
  vpMessage->setTypewriterTarget(t_dialogue);
  //vpMessage->setPlainText(vpMessage->toPlainText());
}

void ReplayScene::setCharacter(QString t_name, QString t_emote)
{
  SceneManager::get().RenderTransition();
  vpPlayer->set_file_name(pAOApp->get_character_sprite_idle_path(t_name, t_emote));
  vpPlayer->start();
  vpPlayer->setVisible(!t_name.isEmpty());
  SceneManager::get().AnimateTransition();
}

void ReplayScene::setBackground(QString t_name)
{
  SceneManager::get().execLoadPlayerBackground(t_name);
  setBgPosition("wit");
}

void ReplayScene::setBgPosition(QString t_name)
{
  vpBackground->set_file_name(SceneManager::get().getBackgroundPath(t_name));
  vpBackground->start();
}

void ReplayScene::constructWidgets()
{
  ThemeManager::get().SetWidgetNames({});

  DRGraphicsView *l_viewport = new DRGraphicsView(this);
  ThemeManager::get().autoWidgetDimensions(l_viewport, "viewport", REPLAYS);
  ThemeManager::get().addWidgetName("viewport", l_viewport);



  vpBackground = new DRSceneMovie(pAOApp);
  vpPlayer = new DRCharacterMovie(pAOApp);

  l_viewport->scene()->addItem(vpBackground);
  l_viewport->scene()->addItem(vpPlayer);

  //setBackground(ReplayManager::get().getPlaybackBackground());

  vpBackground->start();

  SceneManager::get().CreateTransition(this, pAOApp, l_viewport);

  //UI Images
  ThemeManager::get().createImageWidget("chatbox", "replay_chatmed.png", true, this);

  //Messagebox
  vpMessage = new TypewriterTextEdit(this);
  ThemeManager::get().autoWidgetDimensions(vpMessage, "replay_message", REPLAYS);
  ThemeManager::get().addWidgetName("replay_message", vpMessage);
  vpMessage->setFrameStyle(QFrame::NoFrame);
  vpMessage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  vpMessage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  vpMessage->setReadOnly(true);
  GameManager::get().SetTypeWriter(vpMessage);
  set_drtextedit_font(vpMessage, "replay_message", REPLAYS_FONTS_INI, pAOApp);

  //Extra Stuff
         //ThemeManager::get().createButtonWidget("return_to_lobby", this);



  ReplayManager::get().loadReplayPlayback("replay_test.json", this);

}


void ReplayScene::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    qDebug() << "Spacebar pressed!";
    ReplayManager::get().progressPlayback();
  }
  else
  {
    QWidget::keyPressEvent(event);
  }
}
