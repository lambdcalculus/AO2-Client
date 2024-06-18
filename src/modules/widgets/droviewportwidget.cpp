#include "droviewportwidget.h"

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

  SceneManager::get().CreateTransition(this, m_AOApp, this);

  //Connections
  connect(m_VpVideo, SIGNAL(finished()), this, SLOT(OnVideoDone()));
  connect(m_VpPlayer, SIGNAL(done()), this, SLOT(OnPreanimDone()));
}

void DROViewportWidget::ProcessIncomingMessage(ICMessageData *t_IncomingMessage)
{
  SceneManager::get().RenderTransition();
  UpdateBackground(t_IncomingMessage->m_AreaPosition);
   m_IncomingMessage = t_IncomingMessage;

  if (!m_VpVideo->isVisible())
  {
    m_VpVideo->show();
  }

  SceneManager::get().AnimateTransition();
  m_VpEffect->stop();
  m_VpVideo->play_character_video(m_IncomingMessage->m_CharacterFolder, m_IncomingMessage->m_VideoName);
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
