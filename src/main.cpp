#include "aoapplication.h"
#include "courtroom.h"
#include "datatypes.h"
#include "debug_functions.h"
#include "lobby.h"
#include "networkmanager.h"

#include <QDir>
#include <QFileInfo>

int main(int argc, char *argv[])
{
#if QT_VERSION > QT_VERSION_CHECK(5, 6, 0)
  // High-DPI support is for Qt version >=5.6.
  // However, many Linux distros still haven't brought their stable/LTS
  // packages up to Qt 5.6, so this is conditional.
  AOApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

#ifdef Q_OS_MACOS
  { // MacOS
    qputenv("QT_MAC_WANTS_LAYER", "1");

    // pathing
    QDir l_mac_path(QCoreApplication::applicationDirPath());
    for (int i = 0; i < 3; ++i) // equivalent of "/../../.."
      l_mac_path.cdUp();
    QDir::setCurrent(l_mac_path.canonicalPath());
  }
#endif

  AOApplication app(argc, argv);

  app.construct_lobby();
#ifdef QT_NO_DEBUG
  app.net_manager->connect_to_master();
#endif
  app.w_lobby->show();

  return app.exec();
}
