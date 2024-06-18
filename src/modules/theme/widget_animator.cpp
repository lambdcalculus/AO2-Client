#include "widget_animator.h"
#include "qtimer.h"




WidgetAnimator::WidgetAnimator(QWidget *t_widget, int t_framerate)
{
  mAnimationPlayer = new DROAnimation;
  mFrameRate = t_framerate;
  mTargetWidget = t_widget;
}

void WidgetAnimator::startAnimation()
{
  mAnimationPlayer->Start(true);
  updateAnimation();
}

void WidgetAnimator::updateAnimation()
{
  mAnimationPlayer->RunAnimation();
  int posX = mAnimationPlayer->GetCurrentValue(ePOS_X);
  int posY = mAnimationPlayer->GetCurrentValue(ePOS_Y);
  mTargetWidget->move(posX, posY);

  if(mAnimationPlayer->GetCurrentlyRunning())
  {
    QTimer::singleShot(1000 / mFrameRate, this, SLOT(updateAnimation()));
  }
}



void WidgetAnimator::addKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout)
{
  mAnimationPlayer->AddKeyframe(time, type, value, fin, fout);
}

void WidgetAnimator::setKeyframes(QVector<DROAnimationKeyframe> t_frames)
{
  mAnimationPlayer->SetKeyframes(t_frames);
}
