#include "graphicobjectanimator.h"
#include "qgraphicsscene.h"
#include "qtimer.h"

#include <modules/managers/game_manager.h>

GraphicObjectAnimator::GraphicObjectAnimator(mk2::GraphicsSpriteItem *t_widget, int t_framerate)
{
  mAnimationPlayer = new DROAnimation;
  mFrameRate = t_framerate;
  mTargetWidget = t_widget;
}

void GraphicObjectAnimator::startAnimation(bool t_loop)
{
  QRectF boundingRect = mTargetWidget->boundingRect();
  QPointF centerBottom(boundingRect.width() / 2.0, boundingRect.height());

  //mTargetWidget->setTransformOriginPoint(centerBottom);
  mTargetWidget->setCurrentAnimation(mAnimationPlayer);
  mAnimationPlayer->CacheAnimation();
  m_StartTick = GameManager::get().getUptime();
  mAnimationPlayer->Start(t_loop);
  //updateAnimation();
}

void GraphicObjectAnimator::updateAnimation()
{
  int l_CurrentTick = GameManager::get().getUptime();
  int l_AnimationTick = l_CurrentTick - m_StartTick;

  if(!mAnimationPlayer->GetCurrentlyRunning()) return;

  float posX = mAnimationPlayer->GetCachedValue(ePOS_X, l_AnimationTick);
  float posY = mAnimationPlayer->GetCachedValue(ePOS_Y, l_AnimationTick);

  float lRotation = mAnimationPlayer->GetCachedValue(eROTATION, l_AnimationTick);
  float lAlpha = mAnimationPlayer->GetCachedValue(eALPHA, l_AnimationTick);

  float lScale = mAnimationPlayer->GetCachedValue(eSCALE, l_AnimationTick);

  mTargetWidget->setX(posX);
  mTargetWidget->setY(posY);

  if(lScale != -11037)
  {
    double l_scale = (lScale / 100);
    mTargetWidget->setScale(l_scale);
  }

  if(lRotation != -11037)
  {
    mTargetWidget->setRotation(lRotation);
  }
  if(lAlpha != -11037)
  {
    if (lAlpha != 0)
    {
      double opacity = (lAlpha / 255.0);
      mTargetWidget->setOpacity(opacity);
    }
    else
    {
      mTargetWidget->setOpacity(0);
    }
  }
}



void GraphicObjectAnimator::addKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout)
{
  mAnimationPlayer->AddKeyframe(time, type, value, fin, fout);
}

void GraphicObjectAnimator::setKeyframes(QVector<DROAnimationKeyframe> t_frames)
{
  if(t_frames.isEmpty()) mTargetWidget->setCurrentAnimation(nullptr);
  mAnimationPlayer->SetKeyframes(t_frames);
}

DROAnimation *GraphicObjectAnimator::getAnimation()
{
  return mAnimationPlayer;
}
