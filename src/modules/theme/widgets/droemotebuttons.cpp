#include "droemotebuttons.h"
#include "courtroom.h"

#include <QWheelEvent>

DROEmoteButtons::DROEmoteButtons(QWidget *parent, AOApplication *p_ao_app)
    : QWidget{parent}
{
  ao_app = p_ao_app;
}

void DROEmoteButtons::wheelEvent(QWheelEvent *event)
{
  int delta = event->angleDelta().y();

  if (delta > 0)
  {
    ao_app->get_courtroom()->on_emote_left_clicked();
  }
  else if (delta < 0)
  {
    ao_app->get_courtroom()->on_emote_right_clicked();
  }

  QWidget::wheelEvent(event);

}
