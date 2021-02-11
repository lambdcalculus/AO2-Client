#pragma once

#include <QMetaType>
#include <QString>

namespace DRAudio
{
enum class Family
{
  FSystem,
  FEffect,
  FMusic,
  FBlip,
};

enum Option
{
  OSuppressed = 0x1,
};
Q_DECLARE_FLAGS(Options, Option)

QString get_bass_error(int p_error_code);
QString get_last_bass_error();
} // namespace DRAudio
Q_DECLARE_METATYPE(DRAudio::Options)
