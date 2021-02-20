#pragma once

#include "draudiostreamfamily.h"

class DRAudioEngine : public QObject
{
  Q_OBJECT

public:
  DRAudioEngine(QObject *p_parent = nullptr);
  ~DRAudioEngine();

  static DRAudioStreamFamily::ptr get_family(DRAudio::Family p_family);
  static QList<DRAudioStreamFamily::ptr> get_family_list();
  static std::int32_t get_volume();
  static DRAudio::Options get_options();
  // option getter
  static bool is_suppressed();

public slots:
  void set_volume(std::int32_t p_volume);
  void set_options(DRAudio::Options p_options);
  // option setter
  void set_suppressed(bool p_enabled);

signals:
  void volume_changed(std::int32_t);
  void options_changed(DRAudio::Options);

private:
  void adjust_options();
  void adjust_volume();
};
