#include "debug_functions.h"

#include <QCheckBox>
#include <QDebug>
#include <QMessageBox>

#include "aoconfig.h"

void drMessageBox(QString p_message, bool p_is_warning)
{
  AOConfig config;
  if (p_is_warning && !config.display_notification(p_message))
    return;

  QMessageBox message;
  message.setWindowTitle(p_is_warning ? "Warning" : "Notice");
  message.setIcon(p_is_warning ? QMessageBox::Warning : QMessageBox::Information);
  message.setText(p_message);
  QCheckBox *show_again = new QCheckBox(&message);
  show_again->setText(QObject::tr("&Show this message again"));
  show_again->setChecked(true);
  message.setCheckBox(show_again);
  message.exec();

  if (!show_again->isChecked())
    config.filter_notification(p_message);
}

void call_notice(QString p_message)
{
  qInfo() << p_message;
  drMessageBox(p_message, false);
}

void call_warning(QString p_message)
{
  qWarning() << "error:" << p_message;
  drMessageBox(p_message, true);
}
