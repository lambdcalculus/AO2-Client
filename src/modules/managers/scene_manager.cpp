#include "pair_manager.h"
#include "scene_manager.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "aoapplication.h"
#include "file_functions.h"
#include "datatypes.h"
#include "modules/background/background_reader.h"
#include "modules/background/legacy_background_reader.h"
#include "modules/managers/variable_manager.h"

SceneManager SceneManager::s_Instance;

void SceneManager::execLoadPlayerBackground(QString t_backgroundName)
{
  m_BackgroundName = t_backgroundName;
  const QString l_backgroundJSONPath = AOApplication::getInstance()->find_asset_path(AOApplication::getInstance()->get_background_path(t_backgroundName) + "/" + "background.json");
  if(file_exists(l_backgroundJSONPath))
  {
    p_BackgroundCurrent = new BackgroundReader();
  }
  else
  {
    p_BackgroundCurrent = new LegacyBackgroundReader();
  }

  p_BackgroundCurrent->execLoadBackground(t_backgroundName);
}

int SceneManager::getBackgroundPosition()
{
  if(p_BackgroundCurrent == nullptr) return 0;
  return p_BackgroundCurrent->getVerticalPosition(0);
}

QString SceneManager::getBackgroundPath(QString t_position)
{
  if(p_BackgroundCurrent == nullptr) return "";
  QString l_filename = p_BackgroundCurrent->getBackgroundFilename(t_position);
  return AOApplication::getInstance()->get_background_sprite_path(m_BackgroundName, l_filename);
}

QString SceneManager::getForegroundPath(QString t_position)
{
  if(p_BackgroundCurrent == nullptr) return "";
  QString l_filename = p_BackgroundCurrent->getForegroundFilename(t_position);
  return AOApplication::getInstance()->get_background_sprite_path(m_BackgroundName, l_filename);
}

DRBackgroundSettings SceneManager::getBackgroundSettings()
{
  if(p_BackgroundCurrent == nullptr) return DRBackgroundSettings();
  return p_BackgroundCurrent->getSettings();
}

AOLabel *SceneManager::CreateTransition(QWidget *parents, AOApplication *ao_app, DRGraphicsView *viewport)
{
  p_WidgetTransition = new AOLabel(parents, ao_app);

  p_WidgetViewport = viewport;

  p_WidgetTransition->move(p_WidgetViewport->x(), p_WidgetViewport->y());
  p_WidgetTransition->resize(p_WidgetViewport->width(), p_WidgetViewport->height());

  return p_WidgetTransition;
}

AOLabel *SceneManager::GetTransition()
{
  return p_WidgetTransition;
}

void SceneManager::RenderTransition()
{
  QImage image(p_WidgetViewport->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  p_WidgetViewport->scene()->render(&painter);
  p_WidgetTransition->setPixmap(QPixmap::fromImage(image));
}

void SceneManager::AnimateTransition()
{
  QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
  p_WidgetTransition->setGraphicsEffect(eff);

  QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
  a->setDuration(m_FadeDuration);
  a->setStartValue(1);
  a->setEndValue(0);
  a->setEasingCurve(QEasingCurve::OutBack);
  a->start(QPropertyAnimation::DeleteWhenStopped);
}

void SceneManager::setFadeDuration(int duration)
{
  m_FadeDuration = duration;
}

void SceneManager::clearPlayerDataList()
{
  mPlayerDataList.clear();
}

void SceneManager::setCurrentSpeaker(QString t_chara, QString t_emote, int t_type)
{
  m_SpeakerLast = SpeakerData(m_SpeakerCurrent.mCharacter, m_SpeakerCurrent.mEmote);
  m_SpeakerCurrent = SpeakerData(t_chara, t_emote);
  m_SpeakerType = t_type;
  VariableManager::get().setVariable("speaker", m_SpeakerCurrent.mCharacter);
  VariableManager::get().setVariable("speaker_last", m_SpeakerLast.mCharacter);
  VariableManager::get().setVariable("speaker_type", QString::number(t_type));
}

SpeakerData SceneManager::getSpeakerCurrent()
{
  return m_SpeakerCurrent;
}

SpeakerData SceneManager::getSpeakerPrevious()
{
  return m_SpeakerLast;
}

QString SceneManager::getChatboxType()
{

  if(m_SpeakerType == 1) return "shout";
  if(m_SpeakerType == 2) return "think";
  if(m_SpeakerType == 3) return "cg";

  if(!PairManager::get().GetUsePairData()) return "center";
  else
  {
    double l_offset_self = PairManager::get().GetOffsetSelf();
    double l_offset_pair = PairManager::get().GetOffsetOther();

    if(l_offset_self > l_offset_pair) return "right";
    else return "left";
  }
  return "center";
}

int SceneManager::getSpeakerType()
{
  return m_SpeakerType;
}

