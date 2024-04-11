#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H
#include <datatypes.h>
#include <qstring.h>

class CharacterData
{
public:
  CharacterData();

  virtual QString getEmoteButton(DREmote t_emote, bool t_enabled);
  virtual QString getSelectedImage(DREmote t_emote);
  virtual QString getShowname();
  virtual QString getGender();
  virtual QString getSide();
  virtual QStringList getOutfitNames();
  virtual QVector<DREmote> getEmotes();
  virtual double getHeight();

  virtual void loadCharacter(QString t_folder);
  virtual void switchOutfit(QString t_outfit);

  QString mOutfit = "default";
  QString mFolder = "Makoto Naegi (DRO)";
  QString mShowname = "Makoto Naegi";
  QString mGender = "male";
  QString mSide = "wit";

private:

};

#endif // CHARACTERDATA_H
