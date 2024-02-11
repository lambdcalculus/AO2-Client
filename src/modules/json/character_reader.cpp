#include "character_reader.h"
#include "aoapplication.h"

CharacterReader::CharacterReader()
{

}

void CharacterReader::SetCharcter(QString character)
{
  mCharacterFolder = character;
  ReadFromFile(AOApplication::getInstance()->get_character_path(character, "char.json"));
}

QString CharacterReader::getShowname()
{
  return getStringValue("showname");
}

QString CharacterReader::getPosition()
{
  return getStringValue("position");
}

QString CharacterReader::getGender()
{
  return getStringValue("gender");
}

QStringList CharacterReader::getOutfitFolders()
{

}


