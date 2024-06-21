#include "droviewportwidget.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <modules/theme/thememanager.h>
#include "aoapplication.h"
#include <modules/managers/audio_manager.h>
#include <modules/managers/scene_manager.h>

DROViewportWidget::DROViewportWidget(QWidget *parent) : DRGraphicsView{parent}
{
  m_AOApp = AOApplication::getInstance();
}

void DROViewportWidget::ConstructViewport(ThemeSceneType t_scene)
{
  //Create the items that will populate the GraphicsView.
  m_VpBackground = new DRSceneMovie(m_AOApp);
  m_VpPlayer = new DRCharacterMovie(m_AOApp);
  m_VpEffect = new DREffectMovie(m_AOApp);
  m_VpVideo = new DRVideoScreen(m_AOApp);

  //Add items to the scene.
  this->scene()->addItem(m_VpBackground);
  this->scene()->addItem(m_VpPlayer);
  this->scene()->addItem(m_VpEffect);
  this->scene()->addItem(m_VpVideo);

  m_VpBackground->start();
  m_VpPlayer->start();

  m_WidgetTransition = new AOLabel(this, m_AOApp);
  m_WidgetTransition->resize(this->width(), this->height());

  //Connections
  connect(m_VpVideo, SIGNAL(finished()), this, SLOT(OnVideoDone()));
  connect(m_VpPlayer, SIGNAL(done()), this, SLOT(OnPreanimDone()));

  ConstructUserInterface();
}

void DROViewportWidget::ProcessIncomingMessage(ICMessageData *t_IncomingMessage)
{
  TransitionRender();
  UpdateBackground(t_IncomingMessage->m_AreaPosition);
   m_IncomingMessage = t_IncomingMessage;

  if (!m_VpVideo->isVisible())
  {
    m_VpVideo->show();
  }

  TransitionAnimate();
  m_VpEffect->stop();
  m_VpVideo->play_character_video(m_IncomingMessage->m_CharacterFolder, m_IncomingMessage->m_VideoName);
}

void DROViewportWidget::TransitionRender()
{
  QImage image(this->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  this->scene()->render(&painter);
  m_WidgetTransition->setPixmap(QPixmap::fromImage(image));
}

void DROViewportWidget::TransitionAnimate()
{
  QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
  m_WidgetTransition->setGraphicsEffect(eff);

  QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
  a->setDuration(m_FadeDuration);
  a->setStartValue(1);
  a->setEndValue(0);
  a->setEasingCurve(QEasingCurve::OutBack);
  a->start(QPropertyAnimation::DeleteWhenStopped);
}

void DROViewportWidget::ConstructUserInterface()
{
  m_UserInterface = new DRGraphicsView(this);
  m_UserInterface->resize(960, 544);

  m_UserInterface->setStyleSheet("background: transparent;");
  m_UserInterface->setBackgroundBrush(Qt::transparent);


  QStringList l_widgetNames = ThemeManager::get().mCurrentThemeReader.GetSceneWidgetNames(SceneTypeViewport);

  for(QString r_name : l_widgetNames)
  {
    DRSceneMovie* l_uiObject = new DRSceneMovie(m_AOApp);
    m_UserInterface->scene()->addItem(l_uiObject);
    l_uiObject->show();
    l_uiObject->set_file_name(m_AOApp->find_theme_asset_path("vp_" + r_name + ".png"));
    l_uiObject->start();
    m_UserInterfaceObjects[r_name] = l_uiObject;
  }

  m_UserInterface->show();

  m_ShoutsPlayer = new KeyframePlayer(this);
  m_ShoutsPlayer->resize(960, 544);
}

void DROViewportWidget::PlayShoutAnimation(QString t_name)
{
  m_ShoutsPlayer->playAnimation(t_name, eAnimationShout);
}

void DROViewportWidget::ToggleChatbox(bool t_state)
{
  if(m_UserInterfaceObjects.contains("chatbox"))
  {
    if(t_state) m_UserInterfaceObjects["chatbox"]->show();
    else m_UserInterfaceObjects["chatbox"]->hide();
  }
}

void DROViewportWidget::UpdateBackground(QString t_position)
{
  m_VpBackground->set_file_name(SceneManager::get().getBackgroundPath(t_position));
  m_VpBackground->start();
}

void DROViewportWidget::OnVideoDone()
{
  emit VideoDone();
  if (m_VpVideo->isVisible()) m_VpVideo->hide();

  if(!m_IncomingMessage->m_PreAnimation.isEmpty())
  {
    m_VpPlayer->set_play_once(true);
    m_VpPlayer->set_file_name(m_AOApp->get_character_sprite_pre_path(m_IncomingMessage->m_CharacterFolder, m_IncomingMessage->m_PreAnimation));
    m_VpPlayer->start();
  }

  QString l_EffectName = m_IncomingMessage->m_EffectData.mName;
  if(l_EffectName == "<NONE>" || l_EffectName.isEmpty())
  {
    m_VpEffect->hide();
  }
  else
  {
    m_VpEffect->show();
    m_VpEffect->set_play_once(!m_IncomingMessage->m_EffectData.mLoops);
    m_VpEffect->set_file_name(m_AOApp->get_effect_anim_path(l_EffectName));
    m_VpEffect->start();

    QString l_overlay_sfx = m_AOApp->get_sfx(l_EffectName);
    AudioManager::get().PlaySFX(l_overlay_sfx);
  }

  AudioManager::get().PlaySFXCharacter(m_IncomingMessage->m_SFXName, m_IncomingMessage->m_CharacterFolder);
}

void DROViewportWidget::OnPreanimDone()
{
  emit PreanimDone();

  if(m_IncomingMessage->m_HideCharacter)
  {
    m_VpPlayer->hide();
  }
  else
  {
    m_VpPlayer->show();
    m_VpPlayer->set_play_once(false);
    m_VpPlayer->set_file_name(m_AOApp->get_character_sprite_idle_path(m_IncomingMessage->m_CharacterFolder, m_IncomingMessage->m_CharacterEmotion));
    m_VpPlayer->start();
  }
}
