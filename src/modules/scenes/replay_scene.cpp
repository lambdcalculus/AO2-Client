#include "replay_scene.h"
#include "commondefs.h"
#include "modules/theme/thememanager.h"
#include "modules/managers/scene_manager.h"
#include "qevent.h"
#include "modules/managers/game_manager.h"

#include <modules/managers/replay_manager.h>

ReplayScene::ReplayScene(AOApplication *p_ao_app, QWidget *parent) : QWidget(parent)
{
  pAOApp = p_ao_app;
  pMusicPlayer = new AOMusicPlayer(pAOApp, this);
  pSfxPlayer = new AOSfxPlayer(pAOApp, this);
  constructWidgets();
  setWindowTitle("Replay");
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_DeleteOnClose, true );
}

ReplayScene::~ReplayScene()
{
  GameManager::get().SetTypeWriter(nullptr);
  pMusicPlayer->forceStop();
  delete pMusicPlayer;
}

void ReplayScene::playSong(QString t_song)
{
  pMusicPlayer->play(t_song);
}

void ReplayScene::setText(QString t_dialogue)
{
  vpMessage->setTypewriterTarget(t_dialogue);
}

void ReplayScene::setMsgOperation(QMap<QString, QString> t_vars)
{
  mMsgVariables = t_vars;
  setText("");
  vpMessageShowname->setText(mMsgVariables["showname"]);
  setCharacter(mMsgVariables["char"], mMsgVariables["emote"], mMsgVariables["pre"]);
  //mReplayScene->setText(mPlaybackReplay[mCurrentPlaybackIndex].mVariables["msg"]);
}

void ReplayScene::setCharacter(QString t_name, QString t_emote, QString t_pre)
{
  ThemeManager::get().getWidget("chatbox")->setVisible(false);
  if(!t_pre.isEmpty())
  {
    vpPlayer->set_play_once(true);
    vpPlayer->set_file_name(pAOApp->get_character_sprite_pre_path(t_name, t_pre));
    vpPlayer->start();
  }

  //Process Effects
  QString l_effect = mMsgVariables["effect"];
  if(l_effect.trimmed().isEmpty() || l_effect == "0" || l_effect == "-1")
  {
    vpEffect->hide();
  }
  else
  {
    vpEffect->show();
    vpEffect->set_play_once(false);
    vpEffect->set_file_name(pAOApp->get_effect_anim_path(l_effect));
    vpEffect->start();

    QString l_overlay_sfx = pAOApp->get_sfx(mMsgVariables["effect"]);
    pSfxPlayer->play_effect(l_overlay_sfx);
  }

  //Player Character SFX
  pSfxPlayer->play_character_effect(t_name, mMsgVariables["sound"]);
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

void ReplayScene::preanim_done()
{
  SceneManager::get().RenderTransition();
  if(!mMsgVariables["msg"].trimmed().isEmpty()) ThemeManager::get().getWidget("chatbox")->setVisible(true);
  setText(mMsgVariables["msg"]);

  if(mMsgVariables["hide"] == "1")
  {
    vpPlayer->hide();
  }
  else
  {
    vpPlayer->show();
    vpPlayer->set_play_once(false);
    vpPlayer->set_file_name(pAOApp->get_character_sprite_idle_path(mMsgVariables["char"], mMsgVariables["emote"]));
    vpPlayer->start();
  }
  SceneManager::get().AnimateTransition();
}

void ReplayScene::constructWidgets()
{
  ThemeManager::get().SetWidgetNames({});

  DRGraphicsView *l_viewport = new DRGraphicsView(this);
  ThemeManager::get().autoWidgetDimensions(l_viewport, "viewport", REPLAYS);
  ThemeManager::get().addWidgetName("viewport", l_viewport);

  vpBackground = new DRSceneMovie(pAOApp);
  vpPlayer = new DRCharacterMovie(pAOApp);
  vpEffect = new DREffectMovie(pAOApp);

  l_viewport->scene()->addItem(vpBackground);
  l_viewport->scene()->addItem(vpPlayer);
  l_viewport->scene()->addItem(vpEffect);

  vpBackground->start();

  SceneManager::get().CreateTransition(this, pAOApp, l_viewport);

  //UI Images
  ThemeManager::get().createImageWidget("chatbox", "replay_chatmed.png", false, this);
  ThemeManager::get().createImageWidget("vp_overlay", "replay_overlay.png", true, this);

  //Messagebox
  vpMessage = new TypewriterTextEdit(ThemeManager::get().getWidget("chatbox"));
  ThemeManager::get().autoWidgetDimensions(vpMessage, "replay_message", REPLAYS);
  ThemeManager::get().addWidgetName("replay_message", vpMessage);
  vpMessage->setFrameStyle(QFrame::NoFrame);
  vpMessage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  vpMessage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  vpMessage->setReadOnly(true);
  GameManager::get().SetTypeWriter(vpMessage);
  set_drtextedit_font(vpMessage, "replay_message", REPLAYS_FONTS_INI, pAOApp);

  //Showname
  vpMessageShowname = new DRTextEdit(ThemeManager::get().getWidget("chatbox"));
  ThemeManager::get().autoWidgetDimensions(vpMessageShowname, "replay_showname", REPLAYS);
  ThemeManager::get().addWidgetName("replay_showname", vpMessageShowname);
  vpMessageShowname->setFrameStyle(QFrame::NoFrame);
  vpMessageShowname->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  vpMessageShowname->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  vpMessageShowname->setReadOnly(true);
  set_drtextedit_font(vpMessageShowname, "replay_showname", REPLAYS_FONTS_INI, pAOApp);

  //Connections
  connect(vpPlayer, SIGNAL(done()), this, SLOT(preanim_done()));

  //Extra Stuff
         //ThemeManager::get().createButtonWidget("return_to_lobby", this);

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
