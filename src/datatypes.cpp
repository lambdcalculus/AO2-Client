#include "datatypes.h"

QMap<dr::Color, dr::ColorInfo> dr::get_default_color_map()
{
  QMap<Color, ColorInfo> default_color_map;

  default_color_map[dr::CWhite] = ColorInfo("White", "#d5d5d5");
  default_color_map[dr::CGreen] = ColorInfo("Green", "#65c856");
  default_color_map[dr::CRed] = ColorInfo("Red", "#ba1518");
  default_color_map[dr::COrange] = ColorInfo("Orange", "#d55900");
  default_color_map[dr::CBlue] = ColorInfo("Blue", "#1588c8");
  default_color_map[dr::CYellow] = ColorInfo("Yellow", "#e7ce4e");
  default_color_map[dr::CPurple] = ColorInfo("Purple", "#f776fd");
  default_color_map[dr::CPink] = ColorInfo("Pink", "#da7c80");

  return default_color_map;
}
