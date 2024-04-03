#ifndef LEGACYCHARACTERREADER_H
#define LEGACYCHARACTERREADER_H

#include "character_data_reader.h"



class LegacyCharacterReader : public CharacterDataReader
{
public:
  LegacyCharacterReader();

public:
  QString getShowname();
  QString getGender();
  QString getSide();
  QString drLookupKey(const QStringList &keyList, const QString &targetKey);
  QVector<DREmote> getEmotes();
};

#endif // LEGACYCHARACTERREADER_H
