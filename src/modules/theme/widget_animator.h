#ifndef WIDGETANIMATOR_H
#define WIDGETANIMATOR_H
#include <QWidget>

#include "droanimation.h"

#include <QObject>

class WidgetAnimator : public QObject
{
  Q_OBJECT
public:
  WidgetAnimator(QWidget *t_widget, int t_framerate);

  void startAnimation();
  void addKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout);
  void setKeyframes(QVector<DROAnimationKeyframe> t_frames);
private slots:
  void updateAnimation();

private:
  int mFrameRate = 60;
  QWidget *mTargetWidget = nullptr;
  DROAnimation *mAnimationPlayer = nullptr;
};

#endif // WIDGETANIMATOR_H
