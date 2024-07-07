#ifndef GRAPHICOBJECTANIMATOR_H
#define GRAPHICOBJECTANIMATOR_H

#include "droanimation.h"

#include <qobject.h>
#include <datatypes.h>
#include <QGraphicsObject>

#include <mk2/graphicsspriteitem.h>

class GraphicObjectAnimator : public QObject
{
  Q_OBJECT
public:
  GraphicObjectAnimator(mk2::GraphicsSpriteItem *t_widget, int t_framerate);

  void startAnimation(bool t_loop);
  void addKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout);
  void setKeyframes(QVector<DROAnimationKeyframe> t_frames);
  DROAnimation *getAnimation();
  void updateAnimation();

private:
  int m_StartTick = 0;

  int mFrameRate = 60;
  mk2::GraphicsSpriteItem *mTargetWidget = nullptr;
  DROAnimation *mAnimationPlayer = nullptr;
};

#endif // GRAPHICOBJECTANIMATOR_H
