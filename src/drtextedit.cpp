#include "drtextedit.h"
#include "debug_functions.h"

#include <QDebug>
#include <QTextBlock>

DRTextEdit::DRTextEdit(QWidget *parent) : QTextEdit(parent)
{
  connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
  // setViewportMargins(30, 30, 30, 30); // margins are in pixels
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

int DRTextEdit::heightSpan()
{
  return document()->size().height();
}

void DRTextEdit::onTextChanged()
{
  int new_height = heightSpan();
  if (new_height == previous_height)
    return;
  previous_height = new_height;
  qDebug() << this << previous_height;
}
