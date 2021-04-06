#include "drpather.h"

#include "courtroom.h"
#include "file_functions.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#if defined Q_OS_MACOS
QString DRPather::get_application_path()
{
  QString oldPath = QDir::currentPath();
  QDir l_mac_path(QCoreApplication::applicationDirPath());
  for (int i = 0; i < 3; ++i) // equivalent of "/../../.."
    l_mac_path.cdUp();
  return l_mac_path.canonicalPath();
}
#else
QString DRPather::get_application_path()
{
  // For other operating systems (Windows, Linux, etc.) the directory is properly set.
  return QDir::currentPath();
}

#endif
