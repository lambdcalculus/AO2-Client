#include "aoapplication.h"
#include "lobby.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <stdio.h>
#include <stdlib.h>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

    output = QString("[%1] %2: %3").arg(now, output, final_msg);
    // Set to true to get full output
    bool super_debug = false;
    if (super_debug)
      output = QString("[%1] %2: %3 (%4:%5, %6)").arg(
            now, output, final_msg, file, QString::number(context.line), function);
    QFile outFile("base/logs/debug.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << output << Qt::endl;
    outFile.close();
}

int main(int argc, char *argv[])
{
  qInstallMessageHandler(myMessageOutput);
  QFile outFile("base/logs/debug.log");
  outFile.resize(0);
  // High-DPI support is for Qt version >=5.6.
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
  bool l_dpi_scaling = true;

  for (int i = 0; i < argc; ++i)
  {
    const QString l_arg(argv[i]);

    if (l_arg == "-noscaling")
    {
      l_dpi_scaling = false;
    }
  }

  if (l_dpi_scaling)
  {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }
  else
  {
    qputenv("QT_FONT_DPI", "96");
    qputenv("QT_SCALE_FACTOR", "1");
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");

    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
  }
#endif

#ifdef Q_OS_MACOS
  { // MacOS
    qputenv("QT_MAC_WANTS_LAYER", "1");
  }
#endif

  AOApplication app(argc, argv);

  app.load_fonts();
  app.construct_lobby();
  app.get_lobby()->show();

  return app.exec();
}
