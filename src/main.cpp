#include "aoapplication.h"

#include "lobby.h"

#include <QDebug>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
  // High-DPI support is for Qt version >=5.6.
  bool l_dpi_scaling = false;

  for (int i = 0; i < argc; ++i)
  {
    const QString l_arg(argv[i]);

    if (l_arg == "-noscaling")
    {
      l_dpi_scaling = false;
    }
  }

  if (l_dpi_scaling)
  {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }
  else
  {
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi);
  }
#endif

#ifdef Q_OS_MACOS
  { // MacOS
    qputenv("QT_MAC_WANTS_LAYER", "1");
  }
#endif

  AOApplication app(argc, argv);

  app.load_fonts();
  app.construct_lobby();
  app.get_lobby()->show();

  return app.exec();
}
