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

  QStringList getObjectNames();
  QString getAnimPath();

  QSizeF getObjectSize(QString t_name);
  QString getImageName(QString t_name);
  QString getObjectMask(QString t_name);

  bool animationLoaded();

private:
  int mFrameRate = 60;
  bool mLoopAnimation = false;
  QString mAnimationPath = "";

  QStringList mObjectNames = {};

  QMap<QString, QSizeF> mObjectSizes = {};
  QMap<QString, QString> mImageNames = {};
  QMap<QString, QString> mVariableImages = {};


  QMap<QString, QString> mObjectMasking = {};


  QMap<QString, QVector<DROAnimationKeyframe>> mFrames = {};


  QMap<QString, AnimationVariableTypes> mAnimationTypes =
  {
      {"x", ePOS_X},
      {"y", ePOS_Y},
      {"rotation", eROTATION},
      {"width", eVarWidth},
      {"height", eVarHeight},
      {"scale", eSCALE},
      {"alpha", eALPHA}
  };

  QMap<QString, AnimCurveType> mAnimationCurves =
  {
      {"linear", LINEAR},
      {"ease", EASE},
      {"bezier", BEZIER},
      {"parametric", PARAMETRIC},
      {"none", NONE}
  };
};

#endif // ANIMATIONREADER_H
