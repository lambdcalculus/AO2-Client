#pragma once

#include <QMap>
#include <QObject>

class DRPosition
{
public:
  DRPosition();

  DRPosition(QString p_back, QString p_front);

  ~DRPosition();

  QString get_back();

  QString get_front();

  void set_back(QString p_back);

  void set_front(QString p_front);

private:
  QString m_back;

  QString m_front;
};

class DRPositionReader : public QObject
{
  Q_OBJECT

public:
  DRPositionReader(QObject *parent = nullptr);

  ~DRPositionReader();

  DRPosition get_position(QString p_id);

  void load_file(QString p_filename);

private:
  static const QMap<QString, DRPosition> READONLY_POSITIONS;

  QMap<QString, DRPosition> m_positions;
};
