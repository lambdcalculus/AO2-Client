#pragma once

#include <QString>

class DRAudioError
{
public:
  DRAudioError();
  DRAudioError(QString p_error);

  QString get_error();

private:
  QString m_error;
};
