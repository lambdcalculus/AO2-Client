#include "draudioerror.h"

DRAudioError::DRAudioError()
{}

DRAudioError::DRAudioError(QString p_error) : m_error(QString("[bass]%1").arg(p_error))
{}

QString DRAudioError::get_error()
{
  return m_error;
}
