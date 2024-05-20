#include "game_manager.h"
#include "replay_manager.h"

GameManager GameManager::s_Instance;

void GameManager::StartGameLoop()
{
  StopGameLoop();
  ReplayManager::get().startRecording();
  connect(&mFrameTimer, &QTimer::timeout, this, &GameManager::RunGameLoop);
  mFrameTimer.setInterval(1000 / mFPS);
  mFrameTimer.start();
}

void GameManager::StopGameLoop()
{
  mMessageTypeWriter = nullptr;
  mFrameTimer.stop();
}

void GameManager::SetPlayerAnimation(GraphicObjectAnimator *t_animation)
{
  mPlayerAnimation = t_animation;
}

void GameManager::SetTypeWriter(TypewriterTextEdit *t_writer)
{
  mMessageTypeWriter = t_writer;
}

void GameManager::SetAnimationGroup(AnimTypes t_type, QVector<GraphicObjectAnimator *> t_animations)
{
  mRuntimeAnimation[t_type] = t_animations;
}

void GameManager::RunAnimationLoop(AnimTypes t_type)
{
  if(mMessageTypeWriter != nullptr)
  {
    if(!mMessageTypeWriter->isTextRendered())
    {
      mMessageTypeWriter->progressLetter();
    }
  }
  if(mRuntimeAnimation.contains(t_type))
  {
    bool lAllAnimationsDone = true;
    for(GraphicObjectAnimator * r_anim : mRuntimeAnimation[t_type])
    {
      if(r_anim->getAnimation()->getIsPlaying())
      {
        lAllAnimationsDone = false;
        r_anim->getAnimation()->RunAnimation();
        r_anim->updateAnimation();
      }
    }

    if(lAllAnimationsDone)
    {
      mRuntimeAnimation[t_type] = {};
      if(t_type == eAnimationShout) emit ShoutComplete();
      if(t_type == eAnimationGM) emit JudgeComplete();
    }
  }
}

int GameManager::getUptime()
{
  return mUptime;
}

void GameManager::RunGameLoop()
{
  if(!mFlgUpdateRunning)
  {
    mUptime += 1000 / mFPS;
    mFlgUpdateRunning = true;

    if(mPlayerAnimation != nullptr)
    {
      mPlayerAnimation->getAnimation()->RunAnimation();
      mPlayerAnimation->updateAnimation();
    }

    RunAnimationLoop(eAnimationShout);
    RunAnimationLoop(eAnimationGM);


    mFlgUpdateRunning = false;
    emit FrameComplete();
  }
}
