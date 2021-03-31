#include "drpather.h"

#include "courtroom.h"
#include "file_functions.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#if defined Q_OS_MACOS
void DRPather::correctCurrentPath()
{
  QString oldPath = QDir::currentPath();
  QDir l_mac_path(QCoreApplication::applicationDirPath());
  for (int i = 0; i < 3; ++i) // equivalent of "/../../.."
    l_mac_path.cdUp();
  QDir::setCurrent(l_mac_path.canonicalPath());
  return;
}
#else
void DRPather::correctCurrentPath()
{
  // For other operating systems (Windows, Linux, etc.) the directory is properly set.
  return;
}

#endif
