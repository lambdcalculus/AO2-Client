#include "game_manager.h"
#include "replay_manager.h"

GameManager GameManager::s_Instance;

void GameManager::StartGameLoop()
{
  ReplayManager::get().startRecording();
  connect(&mFrameTimer, &QTimer::timeout, this, &GameManager::RunGameLoop);
  mFrameTimer.setInterval(1000 / mFPS);
  mFrameTimer.start();
}

void GameManager::StopGameLoop()
{
  mFrameTimer.stop();
}

void GameManager::SetPlayerAnimation(GraphicObjectAnimator *t_animation)
{
  mPlayerAnimation = t_animation;
}

void GameManager::SetAnimationGroup(AnimTypes t_type, QVector<GraphicObjectAnimator *> t_animations)
{
  mRuntimeAnimation[t_type] = t_animations;
}

void GameManager::RunAnimationLoop(AnimTypes t_type)
{
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

void GameManager::RunGameLoop()
{
  if(!mFlgUpdateRunning)
  {
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
