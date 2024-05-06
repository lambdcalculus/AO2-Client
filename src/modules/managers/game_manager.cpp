#include "game_manager.h"

GameManager GameManager::s_Instance;

void GameManager::StartGameLoop()
{
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

    if(mRuntimeAnimation.contains(eAnimationShout))
    {
      bool lAllAnimationsDone = true;
      for(GraphicObjectAnimator * r_anim : mRuntimeAnimation[eAnimationShout])
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
        mRuntimeAnimation[eAnimationShout] = {};
        emit ShoutComplete();
      }
    }


    mFlgUpdateRunning = false;
    emit FrameComplete();
  }
}
