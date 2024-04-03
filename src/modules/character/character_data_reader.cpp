#include "aoapplication.h"
#include "character_data_reader.h"

CharacterDataReader::CharacterDataReader()
{

}

void CharacterDataReader::loadCharacter(QString t_folder)
{
  mFolder = t_folder;
  ReadFromFile(AOApplication::getInstance()->get_character_path(t_folder, "char.json"));

  mShowname = getStringValue("showname");
  mGender = getStringValue("gender");
  mSide = getStringValue("side");
}
