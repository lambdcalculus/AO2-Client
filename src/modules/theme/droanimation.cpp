#include "droanimation.h"

DROAnimation::DROAnimation()
{

}

void DROAnimation::RunAnimation()
{
  if (!m_IsRunning || m_AnimationKeyframes.empty()) return;

  m_TimeElapsed = m_AnimationTimer.elapsed();

  while(m_AnimationKeyframes.at(m_KeyframeIndex).Time <= m_TimeElapsed)
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
    if(m_TimeElapsed >= nextTime)
    {
      return lastValue;
    }

           //Okay *now* do math.
    qint64 total_duration = nextTime - lastTime;
    qint64 duration_passed = m_TimeElapsed - lastTime;
    float duration_percantage = (float)duration_passed / (float)total_duration;
    float distance = nextValue - lastValue;
    //Caluclate the value differances
    if(fadeIn == NONE)
    {
      if (m_TimeElapsed >= nextTime)
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
