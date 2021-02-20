#include "datatypes.h"

record_type::record_type(QString p_name, QString p_line, QString p_color, bool p_is_system, bool p_is_music)
    : name(p_name), line(p_line), system(p_is_system), music(p_is_music)
{
  Q_UNUSED(p_color);
}
