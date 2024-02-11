#include "pair_manager.h"

PairManager PairManager::s_Instance;

void PairManager::SetSlider(QSlider *slider)
{
  pSliderWidget = slider;
}

bool PairManager::GetUsePairData()
{
  return mPairUsed;
}

void PairManager::SetPairData(QString character, QString emote, int selfOffset, int pairOffset, bool flipped)
{
  mPairOffset = pairOffset;
  mSelfOffset = selfOffset;
  mCharacter = character;
  mEmote = emote;
  mPairUsed = true;
  mIsFlipped = flipped;
}

double PairManager::GetOffsetSelf()
{
  return ((double)mSelfOffset / 960) * 960 - 480;
}

double PairManager::GetOffsetOther()
{
  return ((double)mPairOffset / 960) * 960 - 480;
}

QString PairManager::GetEmoteName()
{
  return mEmote;
}

QString PairManager::GetCharacterFolder()
{
  return mCharacter;
}

bool PairManager::GetCharacterFlipped()
{
  return mIsFlipped;
}

void PairManager::DisableUpcomingPair()
{
  mPairUsed = false;
  mPairOffset = 480;
  mSelfOffset = 480;
}

void PairManager::SetUserPair(int partner, int offset)
{
  mLocalPair = partner;
  mLocalOffset = offset;
  pSliderWidget->setValue(mLocalOffset);
  if(partner != -1) pSliderWidget->show();
  else{ pSliderWidget->hide();}
}

bool PairManager::GetCanPair()
{
  return mLocalPair == -1;
}

bool PairManager::GetSpriteIsVisible()
{
  if(mEmote == "../../misc/blank") return false;
  if(mEmote.isEmpty()) return false;

  return mPairUsed;
}
