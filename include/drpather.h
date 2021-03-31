#ifndef DRPATHER_H
#define DRPATHER_H

#include <QObject>

class DRPather : QObject
{
  Q_OBJECT
public:
  DRPather();

  /* @brief Sets the current path (as given by QDir::currentPath()) to be the directory containing the base folder.
   *
   * This function makes running QDir::currentPath() system-independent. The only edge case really is MacOS, where
   * due to the way applications are bundled, makes QDir::currentPath() return nonsensical values by default.
   *
   * @return True if the current path was changed successfully, false if it remained the same.
   */
  static void correctCurrentPath();
};

#endif // DRPATHER_H
