#pragma once

#include <QMap>
#include <QObject>
#include <QPointer>

#include "draudio.h"
#include "draudiostreamfamily.h"

class QTimer;

#include <optional>

class DRAudioEnginePrivate : public QObject
{
  Q_OBJECT

public:
  using ptr = QPointer<DRAudioEnginePrivate>;

  QObjectList children;
  QPointer<DRAudioEngine> engine;

  QPointer<QTimer> update_timer;
  QVector<DRAudioDevice> device_list;
  std::optional<DRAudioDevice> device;
  QString favorite_device_driver;
  std::optional<DRAudioDevice> favorite_device;

  int32_t volume = 0;
  DRAudio::Options options;
  QMap<DRAudio::Family, DRAudioStreamFamily::ptr> family_map;

  DRAudioEnginePrivate();
  ~DRAudioEnginePrivate();

  void invoke_signal(QString p_method_name, QGenericArgument p_arg1 = QGenericArgument(nullptr));

public slots:
  void update_device_list();
  void update_current_device();
  void update_volume();
  void update_options();
};
