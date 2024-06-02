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
  mBackgroundName = t_backgroundName;
  const QString l_backgroundJSONPath = AOApplication::getInstance()->find_asset_path(AOApplication::getInstance()->get_background_path(t_backgroundName) + "/" + "background.json");
  if(file_exists(l_backgroundJSONPath))
  {
    pCurrentBackground = new BackgroundReader();
  }
  else
  {
    pCurrentBackground = new LegacyBackgroundReader();
  }

  pCurrentBackground->execLoadBackground(t_backgroundName);
}

int SceneManager::getBackgroundPosition()
{
  if(pCurrentBackground == nullptr) return 0;
  return pCurrentBackground->getVerticalPosition(0);
}

QString SceneManager::getBackgroundPath(QString t_position)
{
  if(pCurrentBackground == nullptr) return "";
  QString l_filename = pCurrentBackground->getBackgroundFilename(t_position);
  return AOApplication::getInstance()->get_background_sprite_path(mBackgroundName, l_filename);
}

QString SceneManager::getForegroundPath(QString t_position)
{
  if(pCurrentBackground == nullptr) return "";
  QString l_filename = pCurrentBackground->getForegroundFilename(t_position);
  return AOApplication::getInstance()->get_background_sprite_path(mBackgroundName, l_filename);
}

DRBackgroundSettings SceneManager::getBackgroundSettings()
{
  if(pCurrentBackground == nullptr) return DRBackgroundSettings();
  return pCurrentBackground->getSettings();
}

AOLabel *SceneManager::CreateTransition(QWidget *parents, AOApplication *ao_app, DRGraphicsView *viewport)
{
  pUiTransition = new AOLabel(parents, ao_app);

  pViewport = viewport;

  pUiTransition->move(pViewport->x(), pViewport->y());
  pUiTransition->resize(pViewport->width(), pViewport->height());

  return pUiTransition;
}

AOLabel *SceneManager::GetTransition()
{
  return pUiTransition;
}

void SceneManager::RenderTransition()
{
  QImage image(pViewport->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  pViewport->scene()->render(&painter);
  pUiTransition->setPixmap(QPixmap::fromImage(image));
}

void SceneManager::AnimateTransition()
{
  QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
  pUiTransition->setGraphicsEffect(eff);

  QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
  a->setDuration(mFadeDuration);
  a->setStartValue(1);
  a->setEndValue(0);
  a->setEasingCurve(QEasingCurve::OutBack);
  a->start(QPropertyAnimation::DeleteWhenStopped);
}

void SceneManager::setFadeDuration(int duration)
{
  mFadeDuration = duration;
}

void SceneManager::clearPlayerDataList()
{
  mPlayerDataList.clear();
}

void SceneManager::setCurrentSpeaker(QString t_chara, QString t_emote, int t_type)
{
  mLastSpeaker = SpeakerData(mCurrentSpeaker.mCharacter, mCurrentSpeaker.mEmote);
  mCurrentSpeaker = SpeakerData(t_chara, t_emote);
  mCurrentSpeakerType = t_type;
  VariableManager::get().setVariable("speaker", mCurrentSpeaker.mCharacter);
  VariableManager::get().setVariable("speaker_last", mLastSpeaker.mCharacter);
  VariableManager::get().setVariable("speaker_type", QString::number(t_type));
}

SpeakerData SceneManager::getCurrentSpeaker()
{
  return mCurrentSpeaker;
}

SpeakerData SceneManager::getPreviousSpeaker()
{
  return mLastSpeaker;
}

QString SceneManager::getChatboxType()
{

  if(mCurrentSpeakerType == 1) return "shout";
  if(mCurrentSpeakerType == 2) return "think";
  if(mCurrentSpeakerType == 3) return "cg";

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
  return mCurrentSpeakerType;
}

