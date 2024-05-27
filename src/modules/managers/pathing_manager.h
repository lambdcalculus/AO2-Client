#ifndef PATHINGMANAGER_H
#define PATHINGMANAGER_H

#include <QDir>
#include <QString>
#include "file_functions.h"



class PathingManager
{
public:
  PathingManager(const PathingManager&) = delete;

  static PathingManager& get()
  {
    return s_Instance;
  }

  QString getRunningPath();
  QString getBasePath();
  QString getPackagePath(QString t_package);

  //File / Directory Searching

  QString searchFirstFile(QString t_file);
  QString searchFirstDirectory(QString t_directory);
  QStringList searchAllDirectory(QString t_directory);

  //Packages
  void refreshLocalPackages();
  void refreshDisabledPackages();
  QStringList getPackageNames();
  QStringList getDisabledPackages();

private:
  PathingManager() {}
  static PathingManager s_Instance;

  QStringList mDisabledPackages = {};
  QStringList mLocalPackages = {};
  QStringList mMountedPackages = {};

};

#endif // PATHINGMANAGER_H
