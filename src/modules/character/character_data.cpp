#include "character_data.h"
#include "aoapplication.h"

CharacterData::CharacterData()
{

}

QString CharacterData::getEmoteButton(DREmote t_emote, bool t_enabled)
{
  QString l_texture = AOApplication::getInstance()->get_character_path(t_emote.character, QString("emotions/button%1_off.png").arg(t_emote.key));

  if(t_enabled) l_texture = AOApplication::getInstance()->get_character_path(t_emote.character, QString("emotions/button%1_on.png").arg(t_emote.key));
  qDebug() << l_texture;
  return l_texture;
}

QString CharacterData::getSelectedImage(DREmote t_emote)
{
  QString l_texture = AOApplication::getInstance()->get_character_path(t_emote.character, "emotions/selected.png");
  return l_texture;
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

QStringList CharacterData::getOutfitNames()
{
  return {};
}

QVector<DREmote> CharacterData::getEmotes()
{
  return {};
}

double CharacterData::getHeight()
{
  return 0;
}

void CharacterData::loadCharacter(QString t_folder)
{
  mFolder = t_folder;
}

void CharacterData::switchOutfit(QString t_outfit)
{
  mOutfit = t_outfit;
}
