#include "drcharactermovie.h"

#include "aoapplication.h"
#include "file_functions.h"

#include <QDebug>

DRCharacterMovie::DRCharacterMovie(QWidget *parent) : DRMovie(parent), ao_app(dynamic_cast<AOApplication *>(qApp))
{
  Q_ASSERT(ao_app);
  set_scale_mode(DRMovie::HeightScaling);
}

DRCharacterMovie::~DRCharacterMovie()
{}

void DRCharacterMovie::play(QString p_character, QString p_emote, QString p_prefix, bool p_play_once)
{
  QStringList l_filelist;
  QStringList l_blacklist;
  for (const QString &i_characterName : ao_app->get_char_include_tree(p_character))
  {
    l_blacklist.append({
        ao_app->get_character_path(i_characterName, "char_icon.png"),
        ao_app->get_character_path(i_characterName, "showname.png"),
        ao_app->get_character_path(i_characterName, "emotions"),
    });

    if (!p_play_once)
    {
      l_filelist.append(ao_app->get_character_path(i_characterName, QString("%1%2").arg(p_prefix, p_emote)));
    }

    l_filelist.append(ao_app->get_character_path(i_characterName, p_emote));
  }

  QString l_file = ao_app->find_asset_path(l_filelist, animated_or_static_extensions());
  if (l_file.isEmpty() && !p_play_once)
  {
    l_file = ao_app->find_theme_asset_path("placeholder", animated_extensions());
  }

  for (const QString &i_blackened : qAsConst(l_blacklist))
  {
    if (l_file == i_blackened)
    {
      l_file.clear();
      break;
    }
  }

  if (l_file.isEmpty())
  {
    qWarning() << "error: character animation not found"
               << "character:" << p_character << "emote:" << p_emote << "prefix:" << p_prefix;
  }

  set_file_name(l_file);
  set_play_once(p_play_once);
  start();
}

void DRCharacterMovie::play_pre(QString p_character, QString p_emote)
{
  play(p_character, p_emote, nullptr, true);
}

void DRCharacterMovie::play_talk(QString p_character, QString p_emote)
{
  play(p_character, p_emote, "(b)", false);
}

void DRCharacterMovie::play_idle(QString p_character, QString p_emote)
{
  play(p_character, p_emote, "(a)", false);
}
