#include "droanimation.h"

DROAnimation::DROAnimation()
{

}

void DROAnimation::CacheAnimation()
{
  //Give it an extra frame, just in case something goes wrong.
  m_DurationLength = (GetLengthInFrames() + 16);
  int l_CurrentFrame = 0;

  m_CachedVariables.clear();

  while(m_DurationLength > l_CurrentFrame)
  {
    m_CachedVariables[ePOS_X].append(GetFrameValue(ePOS_X, l_CurrentFrame));
    m_CachedVariables[ePOS_Y].append(GetFrameValue(ePOS_Y, l_CurrentFrame));
    m_CachedVariables[eVarWidth].append(GetFrameValue(eVarWidth, l_CurrentFrame));
    m_CachedVariables[eVarHeight].append(GetFrameValue(eVarHeight, l_CurrentFrame));
    m_CachedVariables[eSCALE].append(GetFrameValue(eSCALE, l_CurrentFrame));
    m_CachedVariables[eKEYFRAME].append(GetFrameValue(eKEYFRAME, l_CurrentFrame));
    m_CachedVariables[eALPHA].append(GetFrameValue(eALPHA, l_CurrentFrame));
    m_CachedVariables[eFIXED_HEIGHT].append(GetFrameValue(eFIXED_HEIGHT, l_CurrentFrame));
    m_CachedVariables[eROTATION].append(GetFrameValue(eROTATION, l_CurrentFrame));

    l_CurrentFrame += 16;
  }

  return;
}

float DROAnimation::GetFrameValue(AnimationVariableTypes t_frameType, int t_frame)
{
  if(m_AnimationKeyframes.count() == 0) return 0;

  DROAnimationKeyframe l_lastKeyframe = DROAnimationKeyframe(0, eNONE, 0, LINEAR,  LINEAR);
  DROAnimationKeyframe l_upcomingKeyframe = DROAnimationKeyframe(0, eNONE, 0, LINEAR,  LINEAR);

  //Get the two relevant keyframes.
  for(DROAnimationKeyframe r_Keyframe : m_AnimationKeyframes)
  {
    if(r_Keyframe.Type == t_frameType)
    {
      if(r_Keyframe.Time < t_frame)
      {
        l_lastKeyframe = r_Keyframe;
      }
      else if(r_Keyframe.Time >= t_frame)
      {
        l_upcomingKeyframe = r_Keyframe;
        break;
      }
    }
  }

  bool l_upcomingKeyframeFound = (l_upcomingKeyframe.Type != eNONE);
  bool l_previousKeyframeFound = (l_lastKeyframe.Type != eNONE);


  if(!l_previousKeyframeFound)
  {
    //Return the upcoming keyframe value if no previous keyframes exist.
    if(l_upcomingKeyframeFound) return l_upcomingKeyframe.Value;

    //Return a fallback value because no value was specified for this parameter.
    else return -11037;
  }

  if(!l_upcomingKeyframeFound)
  {
    //Return the previous keyframe value if no new keyframes exist.
    if(l_previousKeyframeFound) return l_lastKeyframe.Value;

    //Return a fallback value because no value was specified for this parameter.
    else return -11037;
  }

  //Return the upcoming keyframe value if the target frame matches exactly with the upcoming frame.
  if(l_upcomingKeyframe.Time == t_frame)
  {
    return l_upcomingKeyframe.Value;
  }

  qint64 l_TimeFramePrevious = l_lastKeyframe.Time;
  qint64 l_TimeFrameUpcoming = l_upcomingKeyframe.Time;

  AnimCurveType l_FadeTypeIn = l_lastKeyframe.FadeIn;
  AnimCurveType l_FadeTypeOut = l_lastKeyframe.FadeOut;

  float l_FrameValuePrevious = l_lastKeyframe.Value;
  float l_FrameValueUpcoming = l_upcomingKeyframe.Value;

  //Since NONE requires the least amount of processing, we'll check for it first.
  if(l_FadeTypeIn == NONE)
  {
    if (t_frame >= l_TimeFrameUpcoming) return l_FrameValueUpcoming;
    else return l_FrameValuePrevious;
  }

  //Calculate the required variables for the upcoming checks.
  qint64 l_KeyframeDurationLength = l_TimeFrameUpcoming - l_TimeFramePrevious;
  qint64 l_KeyframeDurationPassed = t_frame - l_TimeFramePrevious;
  float l_KeyframeDurationPercentage = (float)l_KeyframeDurationPassed / (float)l_KeyframeDurationLength;
  float l_KeyframeValueTraveled = l_FrameValueUpcoming - l_FrameValuePrevious;

  //Do animation curve math.
  if(l_KeyframeDurationPercentage <= 0.5)
  {
    if(l_FadeTypeIn == EASE)
    {
      l_KeyframeDurationPercentage = 2 * l_KeyframeDurationPercentage * l_KeyframeDurationPercentage;
    }
    if(l_FadeTypeIn == BEZIER)
    {
      l_KeyframeDurationPercentage = l_KeyframeDurationPercentage * l_KeyframeDurationPercentage * (3.0f - 2.0f * l_KeyframeDurationPercentage);
    }
    if(l_FadeTypeIn == PARAMETRIC)
    {
      l_KeyframeDurationPercentage = (l_KeyframeDurationPercentage * l_KeyframeDurationPercentage) / (2.0f * ((l_KeyframeDurationPercentage * l_KeyframeDurationPercentage) - l_KeyframeDurationPercentage) + 1.0f);
    }
  }
  else
  {
    if(l_FadeTypeOut == EASE)
    {
      l_KeyframeDurationPercentage -= 0.5;
      l_KeyframeDurationPercentage = 2 * l_KeyframeDurationPercentage * (1 - l_KeyframeDurationPercentage) + 0.5;
    }
    if(l_FadeTypeIn == BEZIER)
    {
      l_KeyframeDurationPercentage = l_KeyframeDurationPercentage * l_KeyframeDurationPercentage * (3.0f - 2.0f * l_KeyframeDurationPercentage);
    }
    if(l_FadeTypeIn == PARAMETRIC)
    {
      l_KeyframeDurationPercentage = (l_KeyframeDurationPercentage * l_KeyframeDurationPercentage) / (2.0f * ((l_KeyframeDurationPercentage * l_KeyframeDurationPercentage) - l_KeyframeDurationPercentage) + 1.0f);
    }
  }


  float distanced_traveled = l_KeyframeValueTraveled * l_KeyframeDurationPercentage;

  return l_FrameValuePrevious + distanced_traveled;

}

void DROAnimation::RunAnimation()
{
  if (!m_IsRunning || m_AnimationKeyframes.empty()) return;

  m_DurationElapsed = m_AnimationTimer.elapsed();

  while(m_AnimationKeyframes.at(m_KeyframeIndex).Time <= m_DurationElapsed)
  {
    //Return and stop the animation if the keyframe count is at the end.
    if(m_AnimationKeyframes.count() == m_KeyframeIndex + 1) { m_IsRunning = false; m_KeyframeIndex += 1;  if(m_AnimationLoops) Start(m_AnimationLoops); return; }

    m_KeyframeIndex += 1;

           //Set the last keyframe for the current variable and look for the next one.
    m_PreviousValues[m_AnimationKeyframes.at(m_KeyframeIndex - 1).Type] = m_UpcomingValues[m_AnimationKeyframes.at(m_KeyframeIndex - 1).Type];
    SetNextKeyframe(m_AnimationKeyframes.at(m_KeyframeIndex - 1).Type);
  }
}

void DROAnimation::SetNextKeyframe(AnimationVariableTypes animType)
{
  for(int i = m_KeyframeIndex; i < m_AnimationKeyframes.count(); i++)
  {
    if(m_AnimationKeyframes.at(i).Type == animType)
    {
      m_UpcomingValues[animType] = i;
      return;
    }
  }
  m_UpcomingValues[animType] = -1;
}

void DROAnimation::Start(bool loop)
{
  Stop();
  if(m_AnimationKeyframes.count() == 0) return;
  m_UpcomingValues = {};
  m_PreviousValues = {};
  m_KeyframeIndex = 0;

  m_AnimationLoops = loop;

  int keyframeIndex = 0;
  for(DROAnimationKeyframe keyframe : m_AnimationKeyframes)
  {
    if(!m_UpcomingValues.contains(keyframe.Type))
    {
      m_UpcomingValues[keyframe.Type] = keyframeIndex;
      m_PreviousValues[keyframe.Type] = -1;
    }

    keyframeIndex += 1;
  }
  m_IsRunning = true;
  m_AnimationTimer.start();

}

void DROAnimation::Stop()
{
  m_IsRunning = false;
  m_KeyframeIndex = 0;
}

void DROAnimation::Pause()
{
  m_IsRunning = false;
}

bool DROAnimation::GetCurrentlyRunning()
{
  return m_IsRunning;
}

float DROAnimation::GetCachedValue(AnimationVariableTypes type, int t_frame)
{
  int l_CurrentTick = t_frame;
  if(l_CurrentTick == -1)
  {
    if(m_DurationElapsed == -1) return -11037;
    l_CurrentTick = m_DurationElapsed;
  }

  int t_arrayValue = l_CurrentTick / m_AnimationTickRate;

  if(m_AnimationLoops) t_arrayValue = t_arrayValue % (m_CachedVariables[type].count() - 1);
  if(m_CachedVariables.contains(type))
  {
    if(m_CachedVariables[type].count() > t_arrayValue)
    {
      return m_CachedVariables[type][t_arrayValue];
    }
  }

  return -11037;
}

void DROAnimation::AddKeyframe(qint64 time, AnimationVariableTypes type, float value, AnimCurveType fin, AnimCurveType fout)
{
  DROAnimationKeyframe keyframe = DROAnimationKeyframe(time, type, value, fin, fout);
  m_AnimationKeyframes.append(keyframe);
}

void DROAnimation::SetKeyframes(QVector<DROAnimationKeyframe> t_frames)
{
  Stop();
  m_AnimationKeyframes = t_frames;
}

float DROAnimation::GetCurrentValue(AnimationVariableTypes type)
{
  if(m_AnimationKeyframes.count() == 0) return 0;
  if(m_UpcomingValues.contains(type))
  {
    //Return last value if no more keyframes.
    if(m_UpcomingValues[type] == -1)
    {
      if(m_PreviousValues[type] != -1)
      {
        return m_AnimationKeyframes.at(m_PreviousValues[type]).Value;
      }
      return 9999999;
    }

           //Return first value if no keyframes yet.
    {
      if(m_PreviousValues[type] == -1)
      {
        if(m_UpcomingValues[type] != -1)
        {
          return m_AnimationKeyframes.at(m_UpcomingValues[type]).Value;
        }
        return 9999999;
      }
    }

           //Do math now that the -1 values are sorted.

    float lastValue = m_AnimationKeyframes.at(m_PreviousValues[type]).Value;
    qint64 lastTime = m_AnimationKeyframes.at(m_PreviousValues[type]).Time;
    float nextValue = m_AnimationKeyframes.at(m_UpcomingValues[type]).Value;
    qint64 nextTime = m_AnimationKeyframes.at(m_UpcomingValues[type]).Time;
    AnimCurveType fadeIn = m_AnimationKeyframes.at(m_PreviousValues[type]).FadeIn;
    AnimCurveType fadeOut = m_AnimationKeyframes.at(m_PreviousValues[type]).FadeOut;

           //Skip math if already at last time
    if(m_DurationElapsed >= nextTime)
    {
      return lastValue;
    }

           //Okay *now* do math.
    qint64 total_duration = nextTime - lastTime;
    qint64 duration_passed = m_DurationElapsed - lastTime;
    float duration_percantage = (float)duration_passed / (float)total_duration;
    float distance = nextValue - lastValue;
    //Caluclate the value differances
    if(fadeIn == NONE)
    {
      if (m_DurationElapsed >= nextTime)
      {
        return nextValue;
      }
      else
      {
        return lastValue;
      }
    }
    else if(duration_percantage <= 0.5)
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

int DROAnimation::GetCurrentFrame()
{
  return m_KeyframeIndex;
}
