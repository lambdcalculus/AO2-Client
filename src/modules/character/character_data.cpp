#include "character_data.h"

CharacterData::CharacterData()
{

}

QString CharacterData::getShowname()
{
  return mShowname;
}

QString CharacterData::getGender()
{
  return mGender;
}

QString CharacterData::getSide()
{
  return mSide;
}

QVector<DREmote> CharacterData::getEmotes()
{
  return {};
}

void CharacterData::loadCharacter(QString t_folder)
{
  mFolder = t_folder;
}
