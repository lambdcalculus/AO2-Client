#include "graphicobjectanimator.h"
#include "qtimer.h"

GraphicObjectAnimator::GraphicObjectAnimator(QGraphicsObject *t_widget, int t_framerate)
{
  mAnimationPlayer = new DROAnimation;
  mFrameRate = t_framerate;
  mTargetWidget = t_widget;
}

void GraphicObjectAnimator::startAnimation(bool t_loop)
{
  mAnimationPlayer->Start(t_loop);
  updateAnimation();
}

void GraphicObjectAnimator::updateAnimation()
{
  mAnimationPlayer->RunAnimation();
  int posX = mAnimationPlayer->getValue(ePOS_X);
  int posY = mAnimationPlayer->getValue(ePOS_Y);
  mTargetWidget->setPos(posX, posY);

  if(mAnimationPlayer->getIsPlaying())
  {
    QTimer::singleShot(1000 / mFrameRate, this, SLOT(updateAnimation()));
  }
}



void GraphicObjectAnimator::addKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout)
{
  mAnimationPlayer->AddKeyframe(time, type, value, fin, fout);
}

void GraphicObjectAnimator::setKeyframes(QVector<DROAnimationKeyframe> t_frames)
{
  mAnimationPlayer->setKeyframes(t_frames);
}
