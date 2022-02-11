#include "log.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QLockFile>
#include <QString>
#include <QTextStream>

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);
static const int C_DEBUG_FILE_MAX_SIZE = 1e5; // 10 MB
static const QString C_DEBUG_FILE = "base/logs/debug.log";
static const QString C_DEBUG_FILE_LOCK = "base/logs/debug.log.lock";
static const QString C_DEBUG_B_FILE = "base/logs/debug_b.log";
static bool s_verbose_logging = false;

QString generate_message(QtMsgType p_type, const QMessageLogContext &p_context, const QString &p_message)
{
  QByteArray l_localMsg = p_message.toLocal8Bit();
  const QString l_fileName(p_context.file);
  const QString l_function(p_context.function);

  QString l_output;
  switch (p_type)
  {
  case QtDebugMsg:
    l_output = QString("D");
    break;
  case QtInfoMsg:
    l_output = QString("I");
    break;
  case QtWarningMsg:
    l_output = QString("W");
    break;
  case QtCriticalMsg:
    l_output = QString("C");
    break;
  case QtFatalMsg:
    l_output = QString("F");
    break;
  }

  const QString raw_msg = l_localMsg.constData();
  QString final_msg;
  // Censor HDID
  if (raw_msg.startsWith("S/S: HI#"))
    final_msg = "S/S: HI#HDID#%";
  else if (raw_msg.startsWith("M/S: HI#"))
    final_msg = "M/S: HI#HDID#%";
  else
    final_msg = raw_msg;

  const QString now = QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz");
  const QString pid = QString::number(QCoreApplication::applicationPid());

  l_output = QString("[%1] [%2] %3: ").arg(now, pid, l_output);
  l_output.append(final_msg);

  if (s_verbose_logging)
    l_output.append(QString(" (%1:%2, %3)").arg(l_fileName, QString::number(p_context.line), l_function));

  return l_output;
}

void save_log_line(QString p_log_line)
{
  QLockFile l_log_lock(C_DEBUG_FILE_LOCK);
  l_log_lock.lock();

  QFile l_log_file(C_DEBUG_FILE);
  if (!l_log_file.open(QFile::WriteOnly | QFile::Append))
  {
    qInstallMessageHandler(0);
    qCritical().noquote() << QString("Failed to open debug.log! error: %1").arg(l_log_file.errorString());
    l_log_lock.unlock();
    return;
  }

  QTextStream in(&l_log_file);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
  in << p_log_line << Qt::endl;
#else
  in << p_log_line << "\n"; // Hopefully Windows users don't compile with <Qt 5.14
#endif
  l_log_file.close();

  // rename the file if now too big
  if (l_log_file.size() >= C_DEBUG_FILE_MAX_SIZE)
  {
    if (l_log_file.exists(C_DEBUG_B_FILE))
      l_log_file.remove(C_DEBUG_B_FILE);
    l_log_file.rename(C_DEBUG_B_FILE);
  }

  l_log_lock.unlock();
}

void DROLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  save_log_line(generate_message(type, context, msg));

  // Call the default handler.
  (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

void set_verbose_logging(bool enabled)
{
  s_verbose_logging = enabled;
}
