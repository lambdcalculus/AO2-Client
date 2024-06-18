#include "graphicobjectanimator.h"
#include "qgraphicsscene.h"
#include "qtimer.h"

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
  mAnimationPlayer->Start(t_loop);
  //updateAnimation();
}

void GraphicObjectAnimator::updateAnimation()
{
  if(!mAnimationPlayer->GetCurrentlyRunning()) return;

  float posX = mAnimationPlayer->GetCurrentValue(ePOS_X);
  float posY = mAnimationPlayer->GetCurrentValue(ePOS_Y);

  float lRotation = mAnimationPlayer->GetCurrentValue(eROTATION);
  float lAlpha = mAnimationPlayer->GetCurrentValue(eALPHA);

  float lScale = mAnimationPlayer->GetCurrentValue(eSCALE);

  mTargetWidget->setX(posX);
  mTargetWidget->setY(posY);

  if(lScale != 9999999)
  {
    double l_scale = (lScale / 100);
    mTargetWidget->setScale(l_scale);
  }

  if(lRotation != 9999999)
  {
    mTargetWidget->setRotation(lRotation);
  }
  if(lAlpha != 9999999)
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
