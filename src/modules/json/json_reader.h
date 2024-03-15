#ifndef JSONREADER_H
#define JSONREADER_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QVector2D>


class JSONReader
{
public:
  JSONReader();
  void ReadFromFile(QString path);
  void ReadFromString(QString data);

  void SetTargetObject(QJsonObject target);
  void SetTargetObject(QString string);

  bool ValueExists(QString value);
  QString getStringValue(QString string);
  bool getBoolValue(QString string);
  int getIntValue(QString string);
  QVector2D getVector2DValue(QString string);
  QJsonArray getArrayValue(QString string);
  QStringList getStringArrayValue(QString string);

  QJsonDocument mDocument;
  QJsonObject mMainObject;
  QJsonObject mTargetObject;
};

#endif // JSONREADER_H
