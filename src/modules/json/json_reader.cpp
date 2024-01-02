#include "json_reader.h"

JSONReader::JSONReader()
{

}

void JSONReader::ReadFromFile(QString path)
{
  QFile json_file(path); json_file.open(QIODevice::ReadOnly | QIODevice::Text);
  QString fileContents = json_file.readAll(); json_file.close();

  ReadFromString(fileContents);
}

void JSONReader::ReadFromString(QString data)
{
  mDocument = QJsonDocument::fromJson(data.toUtf8());
  mMainObject = mDocument.object();
  mTargetObject = mMainObject;
}

void JSONReader::SetTargetObject(QJsonObject target)
{
  mTargetObject = target;
}

QString JSONReader::getStringValue(QString string)
{
  return mTargetObject.value(string).toString();
}

QVector2D JSONReader::getVector2DValue(QString string)
{
  QVector2D return_data = {};
  return_data.setX(mTargetObject.value(string).toObject().value("x").toDouble());
  return_data.setY(mTargetObject.value(string).toObject().value("y").toDouble());
  return return_data;
}

QJsonArray JSONReader::getArrayValue(QString string)
{
return mTargetObject.value(string).toArray();
}

QVector<QString> JSONReader::getStringArrayValue(QString string)
{
  QVector<QString> return_data = {};
  QJsonArray data = mTargetObject.value(string).toArray();

  for(QJsonValueRef ref : data)
  {
    return_data.append(ref.toString());
  }
  return return_data;
}
