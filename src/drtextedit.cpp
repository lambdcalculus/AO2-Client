#include "drtextedit.h"
#include "debug_functions.h"

#include <QDebug>
#include <QTextBlock>

DRTextEdit::DRTextEdit(QWidget *parent) : QTextEdit(parent)
{
  connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void DRTextEdit::setOutline(bool outline)
{
  QTextCharFormat widget_format = currentCharFormat();
  if (outline)
    widget_format.setTextOutline(QPen(Qt::black, 1));
  else
    widget_format.setTextOutline(Qt::NoPen);
  setCurrentCharFormat(widget_format);

  this->_outline = outline;
}

bool DRTextEdit::outline()
{
  return this->_outline;
}

void DRTextEdit::setVerticalAlignment(Qt::Alignment verticalAlignment)
{
  // If an invalid vertical alignment is passed, convert to Top.
  if (verticalAlignment != Qt::AlignTop && verticalAlignment != Qt::AlignVCenter &&
      verticalAlignment != Qt::AlignBottom)
    verticalAlignment = Qt::AlignTop;
  this->_verticalAlignment = verticalAlignment;
}

Qt::Alignment DRTextEdit::verticalAlignment()
{
  return this->_verticalAlignment;
}

void DRTextEdit::onTextChanged()
{
  // Do computations to align text vertically according to preference
  // We do these computations every time the text changes, so we better be efficient!

  // This stores the total height of the totality of the text saved.
  int new_height = document()->size().height();
  // If the height did not change, exit early, as we don't need to do much formatting
  if (new_height == previous_height)
    return;
  previous_height = new_height;

  // If the height exceeds the height of the widget, aligning vertically does not make much sense
  // so we exit early.
  if (new_height > height())
    return;

  // The way we will simulate vertical alignment is by adjusting the top margin to simulate
  // center alignment, or bottom alignment.
  int topMargin = 0;
  switch (_verticalAlignment)
  {
  case Qt::AlignTop:
    return; // Don't need to do much here
  case Qt::AlignVCenter:
    topMargin = (height() - new_height) / 2;
    break;
  case Qt::AlignBottom:
    topMargin = (height() - new_height);
    break;
  default:
    qDebug() << "Unrecognized vertical alignment " << this->_verticalAlignment << ". Assuming Top.";
    break;
  }

  setViewportMargins(0, topMargin, 0, 0);
}
