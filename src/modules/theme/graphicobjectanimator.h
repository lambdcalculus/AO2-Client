#ifndef GRAPHICOBJECTANIMATOR_H
#define GRAPHICOBJECTANIMATOR_H

#include "droanimation.h"

#include <qobject.h>
#include <datatypes.h>
#include <QGraphicsObject>

class GraphicObjectAnimator : public QObject
{
  Q_OBJECT
public:
  GraphicObjectAnimator(QGraphicsObject *t_widget, int t_framerate);

  void startAnimation(bool t_loop);
  void addKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout);
  void setKeyframes(QVector<DROAnimationKeyframe> t_frames);

private slots:
  void updateAnimation();

private:
  int mFrameRate = 60;
  QGraphicsObject *mTargetWidget = nullptr;
  DROAnimation *mAnimationPlayer = nullptr;
};

#endif // GRAPHICOBJECTANIMATOR_H
