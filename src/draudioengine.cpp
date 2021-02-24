#include "draudioengine.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMap>
#include <QPointer>

class DRAudioEnginePrivate : public QObject
{
  Q_OBJECT

public:
  using ptr = std::unique_ptr<DRAudioEnginePrivate>;

  DRAudioEnginePrivate(QObject *parent = nullptr) : QObject(parent)
  {}

  void invoke_signal(QString p_method_name, QGenericArgument p_arg1 = QGenericArgument(nullptr))
  {
    for (QObject *i_parent : parents)
    {
      QMetaObject::invokeMethod(i_parent, p_method_name.toStdString().c_str(), p_arg1);
    }
  }

  QObjectList parents;
  std::int32_t volume = 0;
  DRAudio::Options options;
  QMap<DRAudio::Family, DRAudioEngine::family_ptr> family_map;
};

namespace
{
static QPointer<DRAudioEnginePrivate> d;
}

DRAudioEngine::DRAudioEngine(QObject *p_parent) : QObject(p_parent)
{
  if (BOOL r = BASS_Init(-1, 48000, BASS_DEVICE_LATENCY, 0, NULL); r == FALSE)
    qWarning() << DRAudioError(QString("failed to init: %1").arg(DRAudio::get_last_bass_error())).get_error();

  if (d == nullptr)
  {
    Q_ASSERT_X(qApp, "initialization", "QApplication is required");
    d = new DRAudioEnginePrivate(qApp);

    // there's plenty of way of optimizing this
    d->family_map.insert(DRAudio::Family::FSystem, family_ptr(new DRAudioStreamFamily(DRAudio::Family::FSystem)));
    d->family_map.insert(DRAudio::Family::FEffect, family_ptr(new DRAudioStreamFamily(DRAudio::Family::FEffect)));
    d->family_map.insert(DRAudio::Family::FMusic, family_ptr(new DRAudioStreamFamily(DRAudio::Family::FMusic)));
    d->family_map.insert(DRAudio::Family::FBlip, family_ptr(new DRAudioStreamFamily(DRAudio::Family::FBlip)));

    // family-specific options
    get_family(DRAudio::Family::FSystem)->set_ignore_suppression(true);
  }

  d->parents.append(this);
}

DRAudioEngine::~DRAudioEngine()
{
  d->parents.removeAll(this);

  BASS_Free();
}

DRAudioEngine::family_ptr DRAudioEngine::get_family(DRAudio::Family p_family)
{
  return d->family_map.value(p_family);
}

QList<DRAudioEngine::family_ptr> DRAudioEngine::get_family_list()
{
  return d->family_map.values();
}

int32_t DRAudioEngine::get_volume()
{
  return d->volume;
}

DRAudio::Options DRAudioEngine::get_options()
{
  return d->options;
}

bool DRAudioEngine::is_suppressed()
{
  return d->options.testFlag(DRAudio::OSuppressed);
}

void DRAudioEngine::set_volume(int32_t p_volume)
{
  p_volume = std::clamp(p_volume, 0, 100);

  if (d->volume == p_volume)
    return;

  d->volume = p_volume;
  adjust_volume();
  Q_EMIT d->invoke_signal("volume_changed", Q_ARG(std::int32_t, d->volume));
}

void DRAudioEngine::set_options(DRAudio::Options p_options)
{
  if (d->options == p_options)
    return;

  d->options = p_options;
  adjust_options();
  Q_EMIT d->invoke_signal("options_changed", Q_ARG(DRAudio::Options, d->options));
}

void DRAudioEngine::set_suppressed(bool p_enabled)
{
  DRAudio::Options options = d->options;
  options.setFlag(DRAudio::OSuppressed, p_enabled);
  set_options(options);
}

void DRAudioEngine::adjust_options()
{
  adjust_volume();
}

void DRAudioEngine::adjust_volume()
{
  for (auto &i_group : d->family_map.values())
    i_group->adjust_volume();
}

// needed by moc
#include "draudioengine.moc"
