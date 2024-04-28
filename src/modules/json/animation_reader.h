#ifndef ANIMATIONREADER_H
#define ANIMATIONREADER_H

#include <QMap>
#include <QString>

#include "json_reader.h"
#include "datatypes.h"

class AnimationReader : public JSONReader
{
public:
  AnimationReader(AnimTypes t_type, QString t_name);
  QVector<DROAnimationKeyframe> getFrames(QString t_objectName);
  bool getCanLoop();

private:
  int mFrameRate = 60;
  bool mLoopAnimation = false;

  QMap<QString, QVector<DROAnimationKeyframe>> mFrames = {};

  QMap<QString, AnimationVariableTypes> mAnimationTypes =
  {
      {"x", ePOS_X},
      {"y", ePOS_Y},
      {"width", eSIZE_X},
      {"height", eSIZE_Y},
  };

  QMap<QString, AnimCurveType> mAnimationCurves =
  {
      {"linear", LINEAR},
      {"ease", EASE},
      {"bezier", BEZIER},
      {"parametric", PARAMETRIC},
  };
};

#endif // ANIMATIONREADER_H
