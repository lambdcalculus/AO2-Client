#ifndef OUTFITREADER_H
#define OUTFITREADER_H
#include "modules/json/json_reader.h"

class OutfitReader : public JSONReader
{
public:
  OutfitReader(QString t_character, QString t_outfit);
};

#endif // OUTFITREADER_H
