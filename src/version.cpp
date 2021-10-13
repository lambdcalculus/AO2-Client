#include "version.h"

#include <QImageReader>
#include <QString>

int get_release_version()
{
  return 1;
}

int get_major_version()
{
  return 0;
}

int get_minor_version()
{
  return 1;
}

QString get_version_string()
{
  return QString::number(get_release_version()) + "." + QString::number(get_major_version()) + "." +
         QString::number(get_minor_version());
}

QString get_about_message()
{
  const bool hasApng = QImageReader::supportedImageFormats().contains("apng");

  QString msg = QString(
    "<h2>Danganronpa Online</h2>"
    "version: %1"
    "<p><b>Source code:</b> "
    "<a href='https://github.com/Chrezm/DRO-Client'>"
    "https://github.com/Chrezm/DRO-Client</a>"
    "<p><b>Development:</b><br>"
    "Cerapter, Elf, Iuvee, Tricky Leifa, Ziella"
    "<p>Based on Attorney Online 2:<br>"
    "<a href='https://github.com/AttorneyOnline/AO2-Client'>"
    "https://github.com/AttorneyOnline/AO2-Client</a>"
    "<p>Running on Qt version %2 with the BASS audio engine.<br>"
    "APNG plugin loaded: %3"
    "<p>Built on %4"
  )
      .arg(get_version_string())
      .arg(QLatin1String(QT_VERSION_STR))
      .arg(hasApng ? "Yes" : "No")
      .arg(QLatin1String(__DATE__));

  return msg;
}
