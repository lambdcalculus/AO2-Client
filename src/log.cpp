#include <QDateTime>
#include <QFile>
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
  QFile outFile("base/logs/debug.log");
  outFile.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream ts(&outFile);
  ts << log_line << Qt::endl;
  outFile.close();
}

void DROLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  const bool super_debug = false; // Set to true for longer log lines
  QString log_line = generate_message(type, context, msg, super_debug);
  save_log_line(log_line);
  // Call the default handler.
  (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}
