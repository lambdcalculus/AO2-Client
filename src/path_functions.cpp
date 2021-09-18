#include "aoapplication.h"

#include "aoconfig.h"
#include "commondefs.h"
#include "courtroom.h"
#include "drpather.h"
#include "file_functions.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

// Copied over from Vanilla.
// As said in the comments there, this is a *super broad* definition.
// Linux is guaranteed to be case-sensitive, however, in case of Mac, it...
// depends. On Mac, it can be toggled. So, should the time ever come to that,
// manually define CASE_SENSITIVE_FILESYSTEM if you're working on a Mac that
// has, well, a case-sensitive filesystem.
#ifdef Q_OS_LINUX
#define CASE_SENSITIVE_FILESYSTEM
#endif

QString AOApplication::get_base_path()
{
  return DRPather::get_application_path() + "/base/";
}

QString AOApplication::get_character_folder_path(QString p_chr)
{
  QString r_path = get_base_path() + "characters/" + p_chr;
  return get_case_sensitive_path(r_path);
}

QString AOApplication::get_character_path(QString p_chr, QString p_file)
{
  const QString r_path = get_character_folder_path(p_chr) + "/" + p_file;
  return get_case_sensitive_path(r_path);
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

QString AOApplication::format_background_path(QString p_identifier)
{
  return get_base_path() + "background/" + p_identifier;
}

QStringList AOApplication::get_available_background_identifier_list()
{
  QStringList l_bg_list;

  if (is_courtroom_constructed)
  {
    const DRAreaBackground l_area_bg = m_courtroom->get_background();

    const QMap<QString, QString> &l_bg_map = l_area_bg.background_tod_map;
    if (ao_config->is_manual_timeofday_selection_enabled())
    {
      const QString l_manual_tod = ao_config->manual_timeofday();
      if (!l_manual_tod.isEmpty() && l_bg_map.contains(l_manual_tod))
        l_bg_list.append(l_bg_map.value(l_manual_tod));
    }

    const QString l_tod = ao_config->timeofday();
    if (!l_tod.isEmpty() && l_bg_map.contains(l_tod))
      l_bg_list.append(l_bg_map.value(l_tod));

    if (!l_area_bg.background.isEmpty())
      l_bg_list.append(l_area_bg.background);

    l_bg_list.append(BACKGROUND_DEFAULT_NAME);
  }

  return l_bg_list;
}

#include <QDebug>

QString AOApplication::get_current_background_path()
{
  QString l_bg_path;

  if (is_courtroom_constructed)
  {
    QStringList l_bg_list = get_available_background_identifier_list();
    for (QString &i_bg : l_bg_list)
    {
      i_bg = get_case_sensitive_path(format_background_path(i_bg));
      if (!dir_exists(i_bg))
        continue;
      l_bg_path = i_bg;
      break;
    }
  }

  return l_bg_path;
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

/**
 * @brief Returns the 'correct' path for the file given as the parameter by
 * trying to match the case of the actual path.
 *
 * @details This function is mostly used on case-sensitive file systems, like
 * ext4, generally used on Linux. On FAT, there is no difference between
 * "file" and "FILE". On ext4, those are two different files. This results in
 * assets that are detected correctly on Windows not being detected on Linux.
 *
 * For this reason, the implementation of this function is system-dependent:
 * on case-insensitive systems, it just returns the parameter itself.
 *
 * @param p_file The path whose casing must be checked against the actual
 * directory structure.
 *
 * @return The parameter path with fixed casing.
 */
QString AOApplication::get_case_sensitive_path(QString p_file)
#ifndef CASE_SENSITIVE_FILESYSTEM
{
  return p_file.replace("//", "/");
}
#else
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

/**
 * @brief Returns the first case-sensitive file that is the combination of one
 * of the given root and extensions, or empty string if no combination exists.
 *
 * @details A root is matched to all given extensions in order before
 * continuing to the next root.
 *
 * @param possible_roots The potential roots the filepath could have.
 * Case-insensitive.
 * @param possible_exts The potential extensions the filepath could have.
 * Case-insensitive.
 *
 * @return The first case-sensitive root+extension path for which a file
 * exists, or an empty string, if not one does.
 */
QString AOApplication::find_asset_path(QStringList p_file_list, QStringList p_extension_list)
{
  for (QString &i_root : p_file_list)
  {
    // We can assume that possible_exts will only be populated with hardcoded strings.
    // Therefore, the only place where sanitize_path could catch something bad is in the root.
    // So, check that now, so we don't need to check later.
    if (!is_safe_path(i_root))
      continue;

    // Check if parent folder actually exists. If it does not, none of the following files would exist
    if (!dir_exists(QFileInfo(i_root).absolutePath()))
      continue;

    for (QString &i_ext : p_extension_list)
    {
      QString full_path = get_case_sensitive_path(i_root + i_ext);
      if (file_exists(full_path))
        return full_path;
    }
  }

  return nullptr;
}

QString AOApplication::find_asset_path(QStringList p_file_list)
{
  return find_asset_path(p_file_list, QStringList{""});
}

QString AOApplication::find_asset_path(QString p_file, QStringList p_extension_list)
{
  return find_asset_path(QStringList{p_file}, p_extension_list);
}

QString AOApplication::find_asset_path(QString p_file)
{
  return find_asset_path(QStringList{p_file});
}

/**
 * @brief Returns the first case-sensitive file in the theme folder that is
 * of the form name+extension, or empty string if it fails.
 *
 * @details The p_exts list is browsed in order. A name+extension file is
 * searched in order in the following directories before checking the next
 * extension:
 * 1. The current time of day folder in the current gamemode folder
 * 2. The current gamemode folder
 * 3. The current time of day folder
 * 4. The current theme folder.
 * The first path that is matched is the one that is returned. If no file
 * is found at all, it returns an empty string.
 *
 * @param p_name Name of the file to look for. Case-insensitive.
 * @param p_exts The potential extensions the filepath could have.
 * Case-insensitive.
 *
 * @return The first case-sensitive root+extension path that corresponds to an
 * actual file, or an empty string, if not one does.
 */
QString AOApplication::find_theme_asset_path(QString p_file, QStringList p_extension_list)
{
  QStringList l_path_list;

  // Only add gamemode and/or time of day if non empty.
  const QString l_gamemode =
      ao_config->is_manual_gamemode_selection_enabled() ? ao_config->manual_gamemode() : ao_config->gamemode();
  const QString l_timeofday =
      ao_config->is_manual_timeofday_selection_enabled() ? ao_config->manual_timeofday() : ao_config->timeofday();
  const QString l_theme_root = get_base_path() + "themes/" + ao_config->theme();

  if (!l_gamemode.isEmpty())
  {
    if (!l_timeofday.isEmpty())
      l_path_list.append(l_theme_root + "/gamemodes/" + l_gamemode + "/times/" + l_timeofday + "/" + p_file);
    l_path_list.append(l_theme_root + "/gamemodes/" + l_gamemode + "/" + p_file);
  }

  if (!l_timeofday.isEmpty())
    l_path_list.append(l_theme_root + "/times/" + l_timeofday + "/" + p_file);

  l_path_list.append(l_theme_root + "/" + p_file);

  // Check if default folder exists. We do this here as it is cheaper than doing it in find_asset_path
  // (as we know there should not be capitalization or folder jumping shenanigans here.
  const QString l_default_theme_path = get_base_path() + "themes/default/";
  if (dir_exists(l_default_theme_path))
    l_path_list.append(l_default_theme_path + p_file);

  return find_asset_path(l_path_list, p_extension_list);
}

QString AOApplication::find_theme_asset_path(QString p_file)
{
  return find_theme_asset_path(p_file, QStringList{""});
}
