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

  // Refresh. To get around the fact both height and length did not change, we set previous_height
  // to an invalid -1. This will be fixed in the onTextChanged() call
  previous_height = -1;
  onTextChanged();
}

Qt::Alignment DRTextEdit::verticalAlignment()
{
  return this->_verticalAlignment;
}

void DRTextEdit::setHorizontalAlignment(Qt::Alignment horizontalAlignment)
{
  // If an invalid horizontal alignment is passed, convert to Left.
  if (horizontalAlignment != Qt::AlignLeft && horizontalAlignment != Qt::AlignHCenter &&
      horizontalAlignment != Qt::AlignRight)
    horizontalAlignment = Qt::AlignLeft;
  this->_horizontalAlignment = horizontalAlignment;
  // Refresh.
  onTextChanged();
}

Qt::Alignment DRTextEdit::horizontalAlignment()
{
  return this->_horizontalAlignment;
}

void DRTextEdit::onTextChanged()
{
  // Avoid recursion
  if (processing_change)
    return;

  processing_change = true;
  _onTextChanged();
  processing_change = false;
}

void DRTextEdit::_onTextChanged()
{
  // Do computations to align text horizontally
  setAlignment(_horizontalAlignment);

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
    break; // Don't need to do much here
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
