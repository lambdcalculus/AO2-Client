#pragma once

#include <QString>

class DRAudiotrackMetadata
{
public:
  static void update_cache();

  DRAudiotrackMetadata();
  DRAudiotrackMetadata(QString file_name);
  ~DRAudiotrackMetadata();

  QString file_name();
  QString title();
  quint64 loop_start();
  quint64 loop_end();

private:
  QString m_file_name;
  QString m_title;
  quint64 m_loop_start = 0;
  quint64 m_loop_end = 0;
};
