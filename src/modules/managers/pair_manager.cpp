#include "pair_manager.h"
#include "commondefs.h"

#include <AOApplication.h>

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

void PairManager::ThemeReload()
{
  mPairChatboxPositions = {};
  QStringList shownameElements = {"showname", "ao2_chatbox", "message", "chat_arrow"};
  QStringList alignmentElements = {"", "_left", "_right"};

  for(QString elementname : shownameElements)
  {
    for(QString elementalighn : alignmentElements)
    {
      QString fullName = elementname + elementalighn;
      pos_size_type sizing = AOApplication::getInstance()->get_element_dimensions(fullName, COURTROOM_DESIGN_INI);
      if(sizing.height != 0 && sizing.width != 0)
      {
        mPairChatboxPositions[fullName] = sizing;
      }
    }
  }

}

pos_size_type PairManager::GetElementAlignment(QString name, QString alighment)
{
  QString alignmentName = name + "_" + alighment;

  if(mPairChatboxPositions.contains(alignmentName)) return mPairChatboxPositions[alignmentName];
  else if(mPairChatboxPositions.contains(name)) return mPairChatboxPositions[name];

  pos_size_type return_value;
  return_value.x = 0;
  return_value.y = 0;
  return_value.width = -1;
  return_value.height = -1;

  return return_value;

}
