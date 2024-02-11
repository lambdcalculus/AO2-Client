#ifndef CHARACTERREADER_H
#define CHARACTERREADER_H

#include "json_reader.h"



class CharacterReader : public JSONReader
{
public:
  CharacterReader();
  void SetCharcter(QString character);

  //Character Data
  QString getShowname();
  QString getPosition();
  QString getGender();

  //Outfits
  QStringList getOutfitFolders();

private:
  QString mCharacterFolder = "";
};

#endif // CHARACTERREADER_H
