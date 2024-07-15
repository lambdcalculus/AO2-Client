#include "animation_reader.h"
#include "aoapplication.h"
#include "file_functions.h"
#include "modules/managers/scene_manager.h"

#include <modules/managers/variable_manager.h>

AnimationReader::AnimationReader(AnimTypes t_type, QString t_name)
{
  QString l_animationPath = "";
  if(t_type == eAnimationPlayer)
  {
    mAnimationPath = AOApplication::getInstance()->get_base_path() + "animations/" + "characters/";
    l_animationPath = mAnimationPath + t_name + ".json";
  }


  if(t_type == eAnimationGM)
  {
    mAnimationPath = AOApplication::getInstance()->get_base_path() + "animations/" + "judge/" + t_name + "/";
    l_animationPath = mAnimationPath + "anim.json";
  }

  if(t_type == eAnimationShout)
  {
    mAnimationPath = AOApplication::getInstance()->get_base_path() + "animations/" + "shouts/" + t_name + "/";
    l_animationPath = mAnimationPath + "anim.json";
  }


  if(file_exists(l_animationPath))
  {
    ReadFromFile(l_animationPath);
    mLoopAnimation = getBoolValue("loop");
    mSoundName = getStringValue("sound");
    mFrameRate = getIntValue("framerate");

    QJsonArray l_animObjectArray = getArrayValue("objects");

    for(QJsonValueRef r_animObject : l_animObjectArray)
    {
      SetTargetObject(r_animObject.toObject());
      QString lObjectName = getStringValue("name");
      QString l_ParamProxy = getStringValue("proxy");
      VariableMappedString l_mappedString = getVarMappedString(lObjectName);
      QString lImageMask = getStringValue("image_mask");

      if(!l_mappedString.mInputString.isEmpty())
      {
        m_ParametersSpritePath[lObjectName] = l_mappedString;
      }

      if(!l_ParamProxy.isEmpty())
      {
        m_ParametersSpriteProxy[lObjectName] = l_ParamProxy;
      }

      if(!lImageMask.isEmpty())
      {
        mObjectMasking[lObjectName] = lImageMask;
      }

      pos_size_type mDefaultPostion = getPositionData("position");

      mFrames[lObjectName].append(DROAnimationKeyframe(0, ePOS_X, mDefaultPostion.x, LINEAR,  LINEAR));
      mFrames[lObjectName].append(DROAnimationKeyframe(0, ePOS_Y, mDefaultPostion.y, LINEAR,  LINEAR));
      if(t_type != eAnimationPlayer)
      {
        mFrames[lObjectName].append(DROAnimationKeyframe(0, eVarWidth, mDefaultPostion.width, LINEAR,  LINEAR));
        mFrames[lObjectName].append(DROAnimationKeyframe(0, eVarHeight, mDefaultPostion.height, LINEAR,  LINEAR));
      }


      QJsonArray lFrames = getArrayValue("frames");
      mObjectNames.append(lObjectName);


      //Parse the Frame Data
      int lTime = 0;
      for(QJsonValueRef r_frame : lFrames)
      {
        SetTargetObject(r_frame.toObject());
        lTime = getIntValue("time");
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

      mFrames[lObjectName].append(DROAnimationKeyframe(lTime + 1, eNONE, 0, LINEAR,  LINEAR));
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

QStringList AnimationReader::getObjectNames()
{
  return mObjectNames;
}

QString AnimationReader::getAnimPath()
{
  return mAnimationPath;
}

QSizeF AnimationReader::getObjectSize(QString t_name)
{
  if(mObjectSizes.contains(t_name)) return mObjectSizes[t_name];
  return QSizeF(960, 544);
}

QString AnimationReader::getImageName(QString t_name)
{
  if(m_ParametersSpritePath.contains(t_name))
  {
    return VariableManager::get().parseVariableString(m_ParametersSpritePath[t_name]);
  }
  if(mImageNames.contains(t_name)) return mImageNames[t_name];
  return t_name + ".png";
}

QString AnimationReader::getObjectMask(QString t_name)
{
  if(mObjectMasking.contains(t_name))
  {
    return mObjectMasking[t_name];
  }
  return "";
}

QString AnimationReader::getObjectProxy(QString t_name)
{
  if(m_ParametersSpriteProxy.contains(t_name))
  {
    return m_ParametersSpriteProxy[t_name];
  }
  return "";
}

bool AnimationReader::animationLoaded()
{
  return mObjectNames.count() != 0;
}
