#ifndef LOG_H
#define LOG_H

#include <QtMessageHandler>

void DROLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOG_H
