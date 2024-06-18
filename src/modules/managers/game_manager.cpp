#include "audio_manager.h"
#include "game_manager.h"
#include "pathing_manager.h"
#include "replay_manager.h"

#include <modules/json/json_reader.h>

GameManager GameManager::s_Instance;

void GameManager::StartGameLoop()
{
  StopGameLoop();
  ReplayManager::get().RecordingStart();
  connect(&m_FrameTimer, &QTimer::timeout, this, &GameManager::RunGameLoop);
  m_FrameTimer.setInterval(1000 / m_FramesPerSecond);
  m_FrameTimer.start();
}

void GameManager::StopGameLoop()
{
  m_WidgetTypeWriter = nullptr;
  m_FrameTimer.stop();
}

void GameManager::SetPlayerAnimation(GraphicObjectAnimator *t_animation)
{
  m_PlayerAnimation = t_animation;
}

void GameManager::SetTypeWriter(TypewriterTextEdit *t_writer)
{
  m_WidgetTypeWriter = t_writer;
}

void GameManager::SetAnimationGroup(AnimTypes t_type, QVector<GraphicObjectAnimator *> t_animations)
{
  m_GraphicObjectAnimations[t_type] = t_animations;
}

void GameManager::RunAnimationLoop(AnimTypes t_type)
{
  if(m_WidgetTypeWriter != nullptr)
  {
    if(!m_WidgetTypeWriter->GetTypingComplete())
    {
      m_WidgetTypeWriter->UpdateDisplay();
    }
  }
  if(m_GraphicObjectAnimations.contains(t_type))
  {
    bool lAllAnimationsDone = true;
    for(GraphicObjectAnimator * r_anim : m_GraphicObjectAnimations[t_type])
    {
      if(r_anim->getAnimation()->GetCurrentlyRunning())
      {
        lAllAnimationsDone = false;
        r_anim->getAnimation()->RunAnimation();
        r_anim->updateAnimation();
      }
    }

    if(lAllAnimationsDone)
    {
      m_GraphicObjectAnimations[t_type] = {};
      if(t_type == eAnimationShout) emit ShoutComplete();
      if(t_type == eAnimationGM) emit JudgeComplete();
    }
  }
}

GameEffectData GameManager::getEffect(QString t_name)
{
  for(GameEffectData rEffectData : m_GameEffects)
  {
    if(t_name == rEffectData.mName) return rEffectData;
  }

  return GameEffectData("<NONE>");
}

GameEffectData GameManager::getEffect(int t_id)
{
  for(GameEffectData rEffectData : m_GameEffects)
  {
    if(t_id == rEffectData.mLegacyId) return rEffectData;
  }

  return GameEffectData("<NONE>");
}

void GameManager::setupGame()
{
  setupGameEffects();
  StartGameLoop();
  AudioManager::get().InitializeAudio();
}

void GameManager::setServerFunctions(QStringList tFunctionList)
{
  m_ServerFeatures.clear();
  m_ServerFeatures = tFunctionList;
}

bool GameManager::usesServerFunction(QString tFunctionName)
{
  return m_ServerFeatures.contains(tFunctionName);
}

void GameManager::setupGameEffects()
{
  JSONReader lEffectsReader = JSONReader();
  lEffectsReader.ReadFromFile(PathingManager::get().getBasePath() +  "effects/default/effects.json");
  m_GameEffects = {};

  QJsonArray lEffectsArray = lEffectsReader.mDocument.array();
  for(QJsonValueRef rEffect : lEffectsArray)
  {
    lEffectsReader.SetTargetObject(rEffect.toObject());
    GameEffectData lEffectData = GameEffectData(lEffectsReader.getStringValue("effect_name"));
    lEffectData.mLoops = lEffectsReader.getBoolValue("loop");
    lEffectData.mIgnoresPair = lEffectsReader.getBoolValue("ignore_pair");
    lEffectData.mLegacyId = lEffectsReader.getIntValue("id");
    m_GameEffects.append(lEffectData);
  }
}

int GameManager::getUptime()
{
  return m_GameUptime;
}

void GameManager::RunGameLoop()
{
  if(!m_IsUpdateRunning)
  {
    m_GameUptime += 1000 / m_FramesPerSecond;
    m_IsUpdateRunning = true;

    if(m_PlayerAnimation != nullptr)
    {
      m_PlayerAnimation->getAnimation()->RunAnimation();
      m_PlayerAnimation->updateAnimation();
    }

    RunAnimationLoop(eAnimationShout);
    RunAnimationLoop(eAnimationGM);


    m_IsUpdateRunning = false;
    emit FrameComplete();
  }
}
