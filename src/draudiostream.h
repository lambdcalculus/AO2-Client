#pragma once

#include "draudio.h"
#include "draudiodevice.h"
#include "draudioerror.h"

#include <bass.h>

#include <QObject>
#include <QStack>

#include <optional>

class DRAudioEngine;
class DRAudioEnginePrivate;
class DRAudioStreamFamily;

class DRAudioStreamSync
{
public:
  HSYNC sync;
  DWORD type;
};

class DRAudioStream : public QObject
{
  Q_OBJECT

  DRAudioStream(DRAudio::Family p_family);

public:
  using ptr = QSharedPointer<DRAudioStream>;

  ~DRAudioStream();

  DRAudio::Family get_family() const;
  QString get_file_name() const;
  bool is_repeatable() const;
  bool is_playing() const;

public slots:
  std::optional<DRAudioError> set_file_name(QString m_file);
  void set_volume(float p_volume);
  void set_repeatable(bool);
  void set_loop(quint64 start, quint64 end);

  void play();
  void stop();

signals:
  void file_name_changed(QString p_file);
  void finished();
  void looped();

public:
  static void CALLBACK end_sync(HSYNC hsync, DWORD ch, DWORD data, void *userdata);
  static void CALLBACK loop_sync(HSYNC hsync, DWORD ch, DWORD data, void *userdata);

private:
  friend class DRAudioStreamFamily;

  enum InitState
  {
    InitError,
    InitNotDone,
    InitFinished,
  };

  DRAudioEngine *m_engine = nullptr;
  DRAudio::Family m_family;
  QString m_file_name;
  InitState m_init_state = InitNotDone;
  HSTREAM m_hstream = 0;
  float m_volume = 0.0f;
  bool m_repeatable = false;
  QWORD m_loop_start = 0;
  QWORD m_loop_start_pos = 0;
  QWORD m_loop_end = 0;
  QWORD m_loop_end_pos = 0;
  HSYNC m_loop_sync = 0;

  bool ensure_init();
  bool ensure_init() const;
  void update_device(DRAudioDevice);
  void init_loop();
  void seek_loop_start();

signals:
  void device_error(QPrivateSignal);
};
