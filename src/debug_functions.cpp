#include "debug_functions.h"

#include <QDebug>
#include <QMessageBox>

void drMessageBox(QString p_message, bool p_error)
{
  QMessageBox l_box;
  l_box.setWindowTitle(p_error ? "Warning" : "Notice");
  l_box.setIcon(p_error ? QMessageBox::Warning : QMessageBox::Information);
  l_box.setText(p_message);
  l_box.exec();
}

void call_notice(QString p_message)
{
  drMessageBox(p_message, false);
  qInfo() << p_message;
}

void call_warning(QString p_message)
{
  drMessageBox(p_message, true);
  qWarning() << "error:" << p_message;
}
