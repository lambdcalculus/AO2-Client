#include <QDateTime>
#include <QFile>
#include <QLockFile>
#include <QString>
#include <QTextStream>

#include "log.h"

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

QString generate_message(QtMsgType type, const QMessageLogContext &context, const QString &msg, const bool super_debug)
{
  QByteArray localMsg = msg.toLocal8Bit();
  const char *file = context.file ? context.file : "";
  const char *function = context.function ? context.function : "";

  QString output;
  switch (type) {
  case QtDebugMsg:
      output = QString("D");
      break;
  case QtInfoMsg:
      output = QString("I");
      break;
  case QtWarningMsg:
      output = QString("W");
      break;
  case QtCriticalMsg:
      output = QString("C");
      break;
  case QtFatalMsg:
      output = QString("F");
      break;
  }
  QString now = QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz");
  const QString raw_msg = localMsg.constData();
  QString final_msg;
  // Censor HDID
  if (raw_msg.startsWith("S/S: HI#"))
    final_msg = "S/S: HI#HDID#%";
  else if (raw_msg.startsWith("M/S: HI#"))
    final_msg = "M/S: HI#HDID#%";
  else
    final_msg = raw_msg;

  if (super_debug)
    output = QString("[%1] %2: %3 (%4:%5, %6)").arg(
          now, output, final_msg, file, QString::number(context.line), function);
  else
    output = QString("[%1] %2: %3").arg(now, output, final_msg);
  return output;
}

void save_log_line(QString log_line)
{
  // Log should only be up to some size, then rename existing log file to something else
  // At any point there should be at most two log files: the current one, and the one
  // generated immediately before.
  int max_size = 100; // 10 MB
  QFile outFileA("base/logs/debug.log");
  if (outFileA.size() > max_size)
  {
    // Prevent concurrency issues
    QLockFile lock_file("base/logs/debugLock");
    lock_file.lock();
    QFile outFileB("base/logs/debugB.log");
    if (outFileB.exists())
      outFileB.remove();
    outFileA.rename("base/logs/debugB.log");
    lock_file.unlock();
  }
  outFileA.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream ts(&outFileA);
  ts << log_line << Qt::endl;
  outFileA.close();
}

void DROLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  const bool super_debug = false; // Set to true for longer log lines
  QString log_line = generate_message(type, context, msg, super_debug);
  save_log_line(log_line);
  // Call the default handler.
  (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}
