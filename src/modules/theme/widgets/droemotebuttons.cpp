#include "droemotebuttons.h"
#include "courtroom.h"
#include "modules/managers/emotion_manager.h"
#include <QWheelEvent>

DROEmoteButtons::DROEmoteButtons(QWidget *parent)
    : QWidget{parent}
{

}

void DROEmoteButtons::wheelEvent(QWheelEvent *event)
{
  int delta = event->angleDelta().y();

  if (delta > 0)
  {
    EmotionManager::get().execEmotePagePrevious();
  }
  else if (delta < 0)
  {
    EmotionManager::get().execEmotePageNext();
  }

  QWidget::wheelEvent(event);

}
