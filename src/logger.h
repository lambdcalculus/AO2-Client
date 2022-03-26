#pragma once

#include <QtMessageHandler>

namespace logger
{
void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void shutdown();
void set_verbose_logging(bool enabled);
} // namespace logger
