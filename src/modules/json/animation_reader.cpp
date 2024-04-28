#include "animation_reader.h"
#include "aoapplication.h"
#include "file_functions.h"

AnimationReader::AnimationReader(AnimTypes t_type, QString t_name)
{

  QString l_animationPath = AOApplication::getInstance()->get_base_path() + "animations/" + "characters/" + t_name + ".json";
  if(file_exists(l_animationPath))
  {
    ReadFromFile(l_animationPath);
    mLoopAnimation = getBoolValue("loop");
    mFrameRate = getIntValue("framerate");

    QJsonArray l_animObjectArray = getArrayValue("objects");

    for(QJsonValueRef r_animObject : l_animObjectArray)
    {
      SetTargetObject(r_animObject.toObject());
      QString lObjectName = getStringValue("name");
      QJsonArray lFrames = getArrayValue("frames");

      for(QJsonValueRef r_frame : lFrames)
      {
        SetTargetObject(r_frame.toObject());
        int lTime = getIntValue("time");
        QString lVarType = getStringValue("var");
        int lValue = getIntValue("value");
        QString lCruveInString = getStringValue("curve_in");
        QString lCruveOutString = getStringValue("curve_out");


        AnimationVariableTypes lVarTypeEnum = eNONE;
        AnimCurveType lCurveIn = LINEAR;
        AnimCurveType lCurveOut = LINEAR;

        if(mAnimationTypes.contains(lVarType.toLower())) lVarTypeEnum = mAnimationTypes[lVarType.toLower()];
        if(mAnimationCurves.contains(lCruveInString.toLower())) lCurveIn = mAnimationCurves[lCruveInString.toLower()];
        if(mAnimationCurves.contains(lCruveOutString.toLower())) lCurveOut = mAnimationCurves[lCruveOutString.toLower()];

        mFrames[lObjectName].append(DROAnimationKeyframe(lTime, lVarTypeEnum, lValue, lCurveIn,  lCurveOut));
      }
    }
  }
}

QVector<DROAnimationKeyframe> AnimationReader::getFrames(QString t_objectName)
{
  if(mFrames.contains(t_objectName)) return mFrames[t_objectName];
  return {};
}

bool AnimationReader::getCanLoop()
{
  return mLoopAnimation;
}
