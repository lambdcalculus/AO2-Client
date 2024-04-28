#include "droanimation.h"

DROAnimation::DROAnimation()
{

}

void DROAnimation::RunAnimation()
{
  if(keyFrames.count() == 0) return;
  mElapsedTime = timer.elapsed();
  while(keyFrames.at(mCurrentKeyframe).Time <= mElapsedTime)
  {
    //Return and stop the animation if the keyframe count is at the end.
    if(keyFrames.count() == mCurrentKeyframe + 1) { mCurrentlyPlaying = false; mCurrentKeyframe += 1;  if(isLoop) Start(isLoop); return; }

    mCurrentKeyframe += 1;

           //Set the last keyframe for the current variable and look for the next one.
    mVarLastKeyframe[keyFrames.at(mCurrentKeyframe - 1).Type] = mVarNextKeyframe[keyFrames.at(mCurrentKeyframe - 1).Type];
    SetNextKeyframe(keyFrames.at(mCurrentKeyframe - 1).Type);
  }
}

void DROAnimation::SetNextKeyframe(AnimationVariableTypes animType)
{
  for(int i = mCurrentKeyframe; i < keyFrames.count(); i++)
  {
    if(keyFrames.at(i).Type == animType)
    {
      mVarNextKeyframe[animType] = i;
      return;
    }
  }
  mVarNextKeyframe[animType] = -1;
}

void DROAnimation::Start(bool loop)
{
  if(keyFrames.count() == 0) return;
  mVarNextKeyframe = {};
  mVarLastKeyframe = {};
  mCurrentKeyframe = 0;

  isLoop = loop;

  int keyframeIndex = 0;
  for(DROAnimationKeyframe keyframe : keyFrames)
  {
    if(!mVarNextKeyframe.contains(keyframe.Type))
    {
      mVarNextKeyframe[keyframe.Type] = keyframeIndex;
      mVarLastKeyframe[keyframe.Type] = -1;
    }

    keyframeIndex += 1;
  }
  mCurrentlyPlaying = true;
  timer.start();

}

void DROAnimation::Stop()
{
  mCurrentlyPlaying = false;
  mCurrentKeyframe = 0;

}

void DROAnimation::Pause()
{
  mCurrentlyPlaying = false;
}

bool DROAnimation::getIsPlaying()
{
  return mCurrentlyPlaying;
}

void DROAnimation::AddKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout)
{
  DROAnimationKeyframe keyframe = DROAnimationKeyframe(time, type, value, fin, fout);
  keyFrames.append(keyframe);
}

void DROAnimation::setKeyframes(QVector<DROAnimationKeyframe> t_frames)
{
  Stop();
  keyFrames = t_frames;
}

float DROAnimation::getValue(AnimationVariableTypes type)
{
  if(keyFrames.count() == 0) return 0;
  if(mVarNextKeyframe.contains(type))
  {
    //Return last value if no more keyframes.
    if(mVarNextKeyframe[type] == -1)
    {
      if(mVarLastKeyframe[type] != -1)
      {
        return keyFrames.at(mVarLastKeyframe[type]).Value;
      }
      return 9999999;
    }

           //Return first value if no keyframes yet.
    {
      if(mVarLastKeyframe[type] == -1)
      {
        if(mVarNextKeyframe[type] != -1)
        {
          return keyFrames.at(mVarNextKeyframe[type]).Value;
        }
        return 9999999;
      }
    }

           //Do math now that the -1 values are sorted.

    float lastValue = keyFrames.at(mVarLastKeyframe[type]).Value;
    qint64 lastTime = keyFrames.at(mVarLastKeyframe[type]).Time;
    float nextValue = keyFrames.at(mVarNextKeyframe[type]).Value;
    qint64 nextTime = keyFrames.at(mVarNextKeyframe[type]).Time;
    AnimCurveType fadeIn = keyFrames.at(mVarLastKeyframe[type]).FadeIn;
    AnimCurveType fadeOut = keyFrames.at(mVarLastKeyframe[type]).FadeOut;

           //Skip math if already at last time
    if(mElapsedTime >= nextTime)
    {
      return lastValue;
    }

           //Okay *now* do math.
    qint64 total_duration = nextTime - lastTime;
    qint64 duration_passed = mElapsedTime - lastTime;
    float duration_percantage = (float)duration_passed / (float)total_duration;
    float distance = nextValue - lastValue;
    //Caluclate the value differances

    if(duration_percantage <= 0.5)
    {
      if(fadeIn == EASE)
      {
        duration_percantage = 2 * duration_percantage * duration_percantage;
      }
      if(fadeIn == BEZIER)
      {
        duration_percantage = duration_percantage * duration_percantage * (3.0f - 2.0f * duration_percantage);
      }
      if(fadeIn == PARAMETRIC)
      {
        duration_percantage = (duration_percantage * duration_percantage) / (2.0f * ((duration_percantage * duration_percantage) - duration_percantage) + 1.0f);
      }
    }
    else
    {
      if(fadeOut == EASE)
      {
        duration_percantage -= 0.5;
        duration_percantage = 2 * duration_percantage * (1 - duration_percantage) + 0.5;
      }
      if(fadeIn == BEZIER)
      {
        duration_percantage = duration_percantage * duration_percantage * (3.0f - 2.0f * duration_percantage);
      }
      if(fadeIn == PARAMETRIC)
      {
        duration_percantage = (duration_percantage * duration_percantage) / (2.0f * ((duration_percantage * duration_percantage) - duration_percantage) + 1.0f);
      }
    }


    float distanced_traveled = distance * duration_percantage;

    return lastValue + distanced_traveled;
  }
  else
  {
    return 9999999;
  }
}
