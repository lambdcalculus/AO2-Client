#include "pathing_manager.h"

#include "character_manager.h"
#include "commondefs.h"
#include "replay_manager.h"


PathingManager PathingManager::s_Instance;

QString PathingManager::getRunningPath()
{
#ifdef Q_OS_MACOS
  QDir l_mac_path(QCoreApplication::applicationDirPath());
  for (int i = 0; i < 3; ++i) // equivalent of "/../../.."
    l_mac_path.cdUp();
  return l_mac_path.canonicalPath();
#else
  return QDir::currentPath();
#endif
}

QString PathingManager::getBasePath()
{
  return getRunningPath() + "/base/";
}

QString PathingManager::getPackagePath(QString t_package)
{
  return getRunningPath() + "/packages/" + t_package + "/";
}

QString PathingManager::searchFirstFile(QString t_file)
{
  return getBasePath() + t_file;
}

QString PathingManager::searchFirstDirectory(QString t_directory)
{
  for (QString r_PackageName : mLocalPackages)
  {
    if(!mDisabledPackages.contains(r_PackageName))
    {
      QString package_path = getPackagePath(r_PackageName) + t_directory;
      if(dir_exists(package_path))
      {
        return package_path;
      }
    }

  }

  return getBasePath() + t_directory;
}

QStringList PathingManager::searchAllDirectory(QString t_directory)
{
  return {};
}

void PathingManager::refreshLocalPackages()
{
  CharacterManager::get().ResetPackages();
  ReplayManager::get().clearPackagesReplays();

  mLocalPackages.clear();

  QString lPackagesPath = getRunningPath() + "/packages/";
  QDir lPackagesQDir(lPackagesPath);

  QList<QFileInfo> packages_fileinfo = lPackagesQDir.entryInfoList();


  for (int i=0; i< packages_fileinfo.size(); i++)
  {
    if (packages_fileinfo.at(i).isDir())
    {
      QString packageBaseName = packages_fileinfo.at(i).baseName();
      if(!packageBaseName.isEmpty())
      {
        mLocalPackages.append(packageBaseName);

        QDir package_dir(packages_fileinfo.at(i).absoluteFilePath() + "/characters");
        if (package_dir.exists())
        {
          QVector<char_type> packageCharacters;
          QStringList character_folders = package_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
          for (const QString &character_folder : character_folders)
          {
            char_type packageChar;
            packageChar.name = character_folder;
            packageCharacters.append(std::move(packageChar));
          }
          CharacterManager::get().SetCharList(packageBaseName, packageCharacters);
        }

        QDir l_replaysDir(packages_fileinfo.at(i).absoluteFilePath() + "/replays");
        if (l_replaysDir.exists())
        {
          QVector<QString> l_replayGroups;
          QStringList l_folderGroups = l_replaysDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
          ReplayManager::get().cachePackageReplays(packageBaseName, l_replayGroups);
        }
      }
    }
  }

  refreshDisabledPackages();
}

void PathingManager::refreshDisabledPackages()
{
  mDisabledPackages.clear();

  const QString l_ini_path = getBasePath() + BASE_PACKAGES_INI;
  QFile l_packages_ini(l_ini_path);
  if (l_packages_ini.open(QFile::ReadOnly))
  {
    QTextStream in(&l_packages_ini);
    while (!in.atEnd())
    {
      QString l_line = in.readLine().trimmed();
      if(mLocalPackages.contains(l_line)) mDisabledPackages.append(l_line);
    }
  }
}

QStringList PathingManager::getPackageNames()
{
  return mLocalPackages;
}

QStringList PathingManager::getDisabledPackages()
{
  return mDisabledPackages;
}
