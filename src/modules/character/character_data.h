#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H
#include <datatypes.h>
#include <qstring.h>

class CharacterData
{
public:
  CharacterData();

  virtual QString getShowname();
  virtual QString getGender();
  virtual QString getSide();
  virtual QVector<DREmote> getEmotes();

  virtual void loadCharacter(QString t_folder);

  QString mFolder = "Makoto Naegi (DRO)";
  QString mShowname = "Makoto Naegi";
  QString mGender = "male";
  QString mSide = "wit";

private:

};

#endif // CHARACTERDATA_H
