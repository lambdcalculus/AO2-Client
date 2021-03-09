#include "drtextedit.h"

DRTextEdit::DRTextEdit(QWidget *parent) : QTextEdit(parent)
{
}

void DRTextEdit::setOutline(bool outline)
{
  QTextCharFormat widget_format = currentCharFormat();
  if (outline)
    widget_format.setTextOutline(QPen(Qt::black, 1));
  else
    widget_format.setTextOutline(Qt::NoPen);
  setCurrentCharFormat(widget_format);
}
