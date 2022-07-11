#pragma once

#include <QMap>
#include <QObject>

class DRPosition
{
public:
  DRPosition();

  DRPosition(QString back, QString front);

  DRPosition(QString back, QString front, QString ambient_sfx);

  ~DRPosition();

  QString get_back();

  QString get_front();

  QString get_ambient_sfx();

  void set_back(QString back);

  void set_front(QString front);

  void set_ambient_sfx(QString ambient_sfx);

private:
  QString m_back;

  QString m_front;

  QString m_ambient_sfx;
};

class DRPositionMap
{
public:
  static const QMap<QString, DRPosition> LEGACY_POSITION_MAP;

  DRPositionMap();
  ~DRPositionMap();

  DRPosition get_position(QString id);

  void set_position(QString id, DRPosition position);

  bool load_file(QString filename);

private:
  QMap<QString, DRPosition> m_position_map;
};
