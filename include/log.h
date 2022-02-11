#ifndef LOG_H
#define LOG_H

#include <QtMessageHandler>

void DROLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void set_verbose_logging(bool enabled);

#endif // LOG_H
