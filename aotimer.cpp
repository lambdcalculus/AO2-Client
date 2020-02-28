#include "aotimer.h"
#include <QDebug>

AOTimer::AOTimer(QWidget* p_parent, AOApplication *p_ao_app) : QWidget(p_parent)
{
  // Original source: https://stackoverflow.com/questions/36679708/how-to-make-a-chronometer-in-qt-c
  ao_app = p_ao_app;

  ui_timer_label = new AOLabel(this, ao_app);
  AOButton *start_button = new AOButton(this, ao_app);
  AOButton *end_button = new AOButton(this, ao_app);

  start_button->setText("Start");
  end_button->setText("End");
  ui_timer_label->setText(QTime(0, 0).toString());
  start_button->move(0, 0);
  end_button->move(0, 40);
  ui_timer_label->move(0, 80);
  ui_timer_label->resize(200, 200);
  this->resize(200, 200);
  start_button->raise();
  end_button->raise();
  ui_timer_label->raise();

  connect(&t, SIGNAL(timeout()), this, SLOT(updateTime()));
  connect(start_button, SIGNAL(clicked()), this, SLOT(start()));
  connect(end_button, SIGNAL(clicked()), &t, SLOT(stop()));
}

void AOTimer::updateTime()
{
  ui_timer_label->setText(c.getTime().toString("mm:ss.zzz"));
}

void AOTimer::start()
{
  ui_timer_label->setText(QTime(0, 0).toString("mm:ss.zzz"));
  c.restart();
  t.start(16);
  qDebug() << "started timer";
}
