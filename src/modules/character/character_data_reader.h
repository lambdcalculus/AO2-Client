#ifndef CHARACTERDATAREADER_H
#define CHARACTERDATAREADER_H

#include "character_data.h"
#include "modules/json/json_reader.h"


class CharacterDataReader : public CharacterData, public JSONReader
{
public:
  CharacterDataReader();

  void loadCharacter(QString t_folder);

};

#endif // CHARACTERDATAREADER_H
