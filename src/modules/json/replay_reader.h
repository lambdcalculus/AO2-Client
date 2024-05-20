#ifndef REPLAYREADER_H
#define REPLAYREADER_H

#include "json_reader.h"



class ReplayReader : public JSONReader
{
public:
  ReplayReader(QString t_path);

  QVector<ReplayOperation> getOperations();

private:
  QVector<ReplayOperation> mCachedOperations   = {};
};

#endif // REPLAYREADER_H
