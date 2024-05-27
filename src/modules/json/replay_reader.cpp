#include "replay_reader.h"
#include "aoapplication.h"
#include "file_functions.h"

ReplayReader::ReplayReader(QString t_path)
{
  QString l_filePath = t_path;
  if(file_exists(l_filePath))
  {
    ReadFromFile(l_filePath);

    QJsonArray l_scriptOperationsArray = getArrayValue("script");
    for(QJsonValueRef r_operation : l_scriptOperationsArray)
    {
      SetTargetObject(r_operation.toObject());

      ReplayOperation l_operation(getStringValue("op"));
      l_operation.mTimestamp = getIntValue("time");
      QStringList l_keys = r_operation.toObject().keys();

      for(QString l_operationKey : l_keys)
      {
        l_operation.mVariables[l_operationKey] = getStringValue(l_operationKey);
      }

      mCachedOperations.append(l_operation);
    }
  }
}

QVector<ReplayOperation> ReplayReader::getOperations()
{
  return mCachedOperations;
}
