#include "aoapplication.h"
#include "courtroom.h"
#include "file_functions.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

// Copied over from Vanilla.
// As said in the comments there, this is a *super broad* definition.
// Linux is guaranteed to be case-sensitive, however, in case of Mac, it...
// depends. On Mac, it can be toggled. So, should the time ever come to that,
// manually define CASE_SENSITIVE_FILESYSTEM if you're working on a Mac that
// has, well, a case-sensitive filesystem.
#if (defined(LINUX) || defined(__linux__))
#define CASE_SENSITIVE_FILESYSTEM
#endif

#ifdef BASE_OVERRIDE
#include "base_override.h"
#endif
QString base_path = "";

QString AOApplication::get_base_path()
{
  if (base_path == "")
  {
#ifdef BASE_OVERRIDE
    base_path = base_override;
#else
    base_path = QDir::currentPath() + "/base/";
#endif
  }
  return base_path;
}

QString AOApplication::get_data_path()
{
  return get_base_path() + "data/";
}

QString AOApplication::get_character_path(QString p_character, QString p_file)
{
  QString path = get_base_path() + "characters/" + p_character + "/" + p_file;
  return get_case_sensitive_path(path);
}

QString AOApplication::get_sounds_path(QString p_file)
{
  QString path = get_base_path() + "sounds/general/" + p_file;
  return get_case_sensitive_path(path);
}

QString AOApplication::get_music_path(QString p_song)
{
  QString path = get_base_path() + "sounds/music/" + p_song;
  return get_case_sensitive_path(path);
}

QString AOApplication::get_background_path(QString p_file)
{
  if (courtroom_constructed)
  {
    return get_case_sensitive_path(w_courtroom->get_background_path(p_file));
  }
  // this function being called when the courtroom isn't constructed makes no
  // sense
  return "";
}

QString AOApplication::get_default_background_path(QString p_file)
{
  QString path = get_base_path() + "background/gs4/" + p_file;
  return get_case_sensitive_path(path);
}

QString AOApplication::get_evidence_path(QString p_file)
{
  QString default_path = get_case_sensitive_path(get_base_path() + "evidence/" + p_file);
  QString alt_path = get_case_sensitive_path(get_base_path() + "items/" + p_file);

  if (QFile(default_path).exists())
    return default_path;
  else
    return alt_path;
}

QString Courtroom::get_background_path(QString p_file)
{
  return ao_app->get_base_path() + "background/" + current_background + "/" + p_file;
}

#ifndef CASE_SENSITIVE_FILESYSTEM
QString AOApplication::get_case_sensitive_path(QString p_file)
{
  return p_file;
}
#else
QString AOApplication::get_case_sensitive_path(QString p_file)
{
  // First, check to see if the file already exists as it is.
  if (QFile(p_file).exists())
    return p_file;

  QFileInfo file(p_file);

  QString file_basename = file.fileName();
  QString file_parent_dir = get_case_sensitive_path(file.absolutePath());

  // Second, does it exist in the new parent directory?
  if (QFile(file_parent_dir + "/" + file_basename).exists())
    return file_parent_dir + "/" + file_basename;

  // In case it doesn't, look through the entries in the parent directory, and
  // try and find it based on a case-insensitive regex search.
  // Note also the fixed string search here. This is so that, for example, music
  // files with parentheses don't get interpreted as grouping for a regex
  // search.
  QRegExp file_rx = QRegExp(file_basename, Qt::CaseInsensitive, QRegExp::FixedString);
  QStringList files = QDir(file_parent_dir).entryList();

  int result = files.indexOf(file_rx);

  if (result != -1)
    return file_parent_dir + "/" + files.at(result);

  // If nothing is found, we let the caller handle that case.
  return file_parent_dir + "/" + file_basename;
}
#endif

QString AOApplication::find_asset_path(QStringList possible_roots, QStringList possible_exts)
{
  for (QString &root : possible_roots)
  {
    for (QString &ext : possible_exts)
    {
      QString full_path = sanitize_path(get_case_sensitive_path(root + ext));
      if (file_exists(full_path))
        return full_path;
    }
  }

  return nullptr;
}

QString AOApplication::find_theme_asset_path(QString p_file, QStringList exts)
{
  QStringList paths{
      get_base_path() + "themes/" + get_theme() + "/gamemodes/" + get_gamemode() + "/times/" + get_timeofday() + "/" +
          p_file,
      get_base_path() + "themes/" + get_theme() + "/gamemodes/" + get_gamemode() + "/" + p_file,
      get_base_path() + "themes/" + get_theme() + "/times/" + get_timeofday() + "/" + p_file,
      get_base_path() + "themes/" + get_theme() + "/" + p_file,
      get_base_path() + "themes/default/" + p_file,
  };

  return find_asset_path(paths, exts);
}
