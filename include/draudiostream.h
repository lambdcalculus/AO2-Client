#pragma once

#include "draudio.h"
#include "draudioerror.h"

#include <bass.h>

#include <QObject>

#include <memory>
#include <optional>
#include <stack>

class DRAudioStreamFamily;

class DRAudioStream : public QObject
{
  Q_OBJECT

public:
  using ptr = QSharedPointer<DRAudioStream>;

  ~DRAudioStream();

  DRAudio::Family get_family();
  std::optional<QString> get_file();

  // state
  bool is_playing();

public slots:
  void play();
  void stop();

  std::optional<DRAudioError> set_file(QString m_file);
  void set_volume(float p_volume);

signals:
  void file_changed(QString p_file);
  void finished();

public:
  static void CALLBACK sync_on_end(HSYNC hsync, DWORD ch, DWORD data, void *userdata);

private:
  friend class DRAudioStreamFamily;

  DRAudioStream(DRAudio::Family p_family);

  // static method
  DRAudio::Family m_family;
  std::optional<QString> m_file;

  // bass
  std::optional<HSTREAM> m_hstream;
  std::stack<HSYNC> m_hsync_stack;
};
