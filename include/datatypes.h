#ifndef DATATYPES_H
#define DATATYPES_H

#include <QMap>
#include <QString>

#include <memory>

struct record_type
{
  QString name;
  QString line;
  QString color;
  bool system = false;
  bool music = false;

  record_type() = default;
  record_type(QString p_name, QString p_line, QString p_color, bool p_is_system, bool p_is_music)
      : name(p_name), line(p_line), system(p_is_system), music(p_is_music)
  {
    Q_UNUSED(p_color);
  }
};

typedef std::shared_ptr<record_type> record_type_ptr;

typedef QVector<record_type_ptr> record_type_array;

struct server_type
{
  QString name;
  QString desc;
  QString ip;
  int port;
};

struct emote_type
{
  QString comment;
  QString preanim;
  QString anim;
  int mod;
  QString sfx_name;
  int sfx_delay;
  int sfx_duration;
};

struct char_type
{
  QString name;
  QString description;
  QString evidence_string;
  bool taken;
};

struct evi_type
{
  QString name;
  QString description;
  QString image;
};

struct chatmessage_type
{
  QString message;
  QString character;
  QString side;
  QString sfx_name;
  QString pre_emote;
  QString emote;
  int emote_modifier;
  int objection_modifier;
  int realization;
  int text_color;
  int evidence;
  int cid;
  int sfx_delay;
  int flip;
};

struct area_type
{
  QString name;
  QString background;
};

struct pos_size_type
{
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
};

enum ChatMessage : int32_t
{
  CMDeskModifier = 0,
  CMPreAnim,
  CMChrName,
  CMEmote,
  CMMessage,
  CMPosition,
  CMSoundName,
  CMEmoteModifier,
  CMChrId,
  CMSoundDelay,
  CMShoutModifier,
  CMEvidenceId,
  CMFlipState,
  CMEffectState,
  CMTextColor,
  CMShowName,
};

namespace DR
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
using SplitBehavior = QString::SplitBehavior;
#else
using SplitBehavior = Qt::SplitBehaviorFlags;
#endif
const SplitBehavior KeepEmptyParts = SplitBehavior::KeepEmptyParts;
const SplitBehavior SkipEmptyParts = SplitBehavior::SkipEmptyParts;

enum VAlign : int32_t
{
  Top,
  Middle,
  Bottom,
};

enum HAlign : int32_t
{
  Left,
  Center,
  Right,
};

enum Color : int32_t
{
  CDefault,
  CGreen,
  CRed,
  COrange,
  CBlue,
  CYellow,
  CPurple,
  CPink,
  CRainbow,

  // aliases
  CWhite = CDefault,
};

struct ColorInfo
{
public:
  ColorInfo() = default;
  ColorInfo(QString p_showname, QString p_code) : name(p_showname.toLower()), showname(p_showname), code(p_code)
  {}

  QString name;
  QString showname;
  QString code;
};

QMap<Color, ColorInfo> get_default_color_map();
} // namespace DR

#endif // DATATYPES_H
