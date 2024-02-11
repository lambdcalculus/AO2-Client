#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include <datatypes.h>



class CharacterManager
{
public:
  CharacterManager(const CharacterManager&) = delete;

  static CharacterManager& get()
  {
    return s_Instance;
  }

  QVector<char_type> GetCharList();
  QVector<char_type> GetCharList(QString package);
  QVector<char_type> GetServerCharList();

  QString GetFilteredCharaName(int id);
  QString GetServerCharaName(int id);



  void SetCharList(QVector<char_type> charList);
  void SetCharList(QString package, QVector<char_type> charList);

  void SetCharaTaken(int id, bool status);

  void AddToFavorites(QString chara);
  void RemoveFromFavorites(QString chara);
  void LoadFavoritesList();
  void SaveFavoritesList();

  int GetAvaliablePersona();

  QStringList GetCharacterPackages();

  QVector<char_type> mFilteredChrList;

  QVector<char_type> mServerCharacters;
  QVector<char_type> mFavoriteCharacters;

  QHash<QString, QVector<char_type>> mPackageCharacters;

  bool GetCharacterInServer(QString name);
  bool GetCharacterInServer(int filterID);
  int GetFilteredId(int Id);
private:
  CharacterManager()
  {

  }
  static CharacterManager s_Instance;

  QHash<QString, bool>CharacterTaken = {};

  QStringList mCharacterPackages = {"Server Characters", "Favorites"};

};

#endif // CHARACTERMANAGER_H
