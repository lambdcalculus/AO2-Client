#include "version.h"

#include <QFile>
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

QString get_resource_file_text(QString filename)
{
  QString data;

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
      return "";
  }
  data = file.readAll();
  file.close();
  return data;
}

QString get_about_message()
{
  const bool hasApng = QImageReader::supportedImageFormats().contains("apng");
  const QString git_branch = get_resource_file_text(":/res/git/git_branch.txt");
  const QString git_hash = get_resource_file_text(":/res/git/git_hash.txt");

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
    "<p>Built on %4<br>"
  )
      .arg(get_version_string())
      .arg(QLatin1String(QT_VERSION_STR))
      .arg(hasApng ? "Yes" : "No")
      .arg(QLatin1String(__DATE__));

  if (git_branch.isEmpty())
    msg += QString("No git branch information available.<br>");
  else
    msg += QString("Built from git branch %1<br>").arg(git_branch);

  if (git_hash.isEmpty())
    msg += QString("No git hash information available.");
  else
    msg += QString("Hash of the latest commit: %1").arg(git_hash);

  return msg;
}
