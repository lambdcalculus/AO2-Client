#include "datatypes.h"

#include <QMap>

QMap<DR::Color, DR::ColorInfo> DR::get_default_color_map()
{
  QMap<Color, ColorInfo> default_color_map;

  default_color_map[DR::CWhite] = ColorInfo("White", "#d5d5d5");
  default_color_map[DR::CGreen] = ColorInfo("Green", "#65c856");
  default_color_map[DR::CRed] = ColorInfo("Red", "#ba1518");
  default_color_map[DR::COrange] = ColorInfo("Orange", "#d55900");
  default_color_map[DR::CBlue] = ColorInfo("Blue", "#1588c8");
  default_color_map[DR::CYellow] = ColorInfo("Yellow", "#e7ce4e");
  default_color_map[DR::CPurple] = ColorInfo("Purple", "#f776fd");
  default_color_map[DR::CPink] = ColorInfo("Pink", "#da7c80");

  return default_color_map;
}

QString DRServerInfo::to_info() const
{
  QString r_info = "Unnamed Server";

  if (!name.isEmpty())
  {
    r_info = name;
  }
  else if (!address.isEmpty())
  {
    r_info = to_address();
  }

  return r_info;
}

QString DRServerInfo::to_address() const
{
  return address + ":" + QString::number(port);
}

bool DRServerInfo::operator==(const DRServerInfo &o) const
{
  return address == o.address && port == o.port;
}

bool DRServerInfo::operator!=(const DRServerInfo &other) const
{
  return !operator==(other);
}
