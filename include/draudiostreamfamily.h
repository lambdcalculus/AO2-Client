#pragma once

#include "draudiostream.h"

#include <QFlags>
#include <QSharedPointer>
#include <QVector>

#include <algorithm>
#include <optional>

class DRAudioEngine;

class DRAudioStreamFamily : public QObject
{
  Q_OBJECT

public:
  using ptr = QSharedPointer<DRAudioStreamFamily>;

  std::optional<DRAudioStream::ptr> create_stream(QString p_file);
  std::optional<DRAudioStream::ptr> play_stream(QString p_file);
  QVector<DRAudioStream::ptr> get_stream_list();

  std::int32_t get_volume();
  std::int32_t get_capacity();
  DRAudio::Options get_options();
  // options getter
  bool is_suppressed();
  bool is_ignore_suppression();

  using iterator = QVector<DRAudioStream::ptr>::iterator;
  iterator begin();
  iterator end();

public slots:
  void set_volume(std::int32_t p_volume);
  void set_capacity(std::int32_t p_capacity);
  void set_options(DRAudio::Options p_options);
  // options setter
  void set_suppressed(bool p_enabled);
  void set_ignore_suppression(bool p_enabled);

signals:
  void volume_changed(std::int32_t);
  void capacity_changed(std::int32_t);
  void options_changed(DRAudio::Options);

private:
  friend class DRAudioEngine;

  DRAudioStreamFamily(DRAudio::Family p_family);

  float calculate_volume();

  void adjust_capacity();
  void adjust_options();
  void adjust_volume();

  DRAudio::Family m_family;
  std::int32_t m_volume = 0;
  std::int32_t m_capacity = 0;
  DRAudio::Options m_options;
  QVector<QSharedPointer<DRAudioStream>> m_stream_list;

private slots:
  void on_stream_stopped();
};
