#include "aotimer.h"
#include <QDebug>

AOTimer::AOTimer(QWidget* p_parent, AOApplication *p_ao_app) : QWidget(p_parent)
{
  // Adapted from: https://stackoverflow.com/questions/36679708/how-to-make-a-chronometer-in-qt-c
  ao_app = p_ao_app;

  ui_timer_label = new AOLabel(this, ao_app);
  /*
  AOButton *set_button = new AOButton(this, ao_app);
  AOButton *resume_button = new AOButton(this, ao_app);
  AOButton *end_button = new AOButton(this, ao_app);
  AOButton *concentrate = new AOButton(this, ao_app);
  AOButton *normal = new AOButton(this, ao_app);
  AOButton *fast_forward = new AOButton(this, ao_app);

  set_button->setText("set");
  resume_button->setText("Resume");
  end_button->setText("Pause");
  concentrate->setText("Concentrate");
  normal->setText("Normal");
  fast_forward->setText("Fast Forward");

  set_button->move(0, 0);
  resume_button->move(0, 20);
  end_button->move(0, 40);
  concentrate->move(0, 60);
  normal->move(0, 80);
  fast_forward->move(0, 100);*/

  ui_timer_label->move(0, 120);
  ui_timer_label->resize(200, 100);
  this->resize(200, 400);
  ui_timer_label->setStyleSheet("QLabel { color : white; }");

  /*
  set_button->raise();
  resume_button->raise();
  end_button->raise();
  concentrate->raise();
  normal->raise();
  fast_forward->raise();
  ui_timer_label->raise();
  */

  connect(&firing_timer, SIGNAL(timeout()), this, SLOT(update_time()));

  /*
  connect(set_button, SIGNAL(clicked()), this, SLOT(set()));
  connect(resume_button, SIGNAL(clicked()), this, SLOT(resume()));
  connect(concentrate, SIGNAL(clicked()), this, SLOT(set_concentrate_mode()));
  connect(normal, SIGNAL(clicked()), this, SLOT(set_normal_mode()));
  connect(fast_forward, SIGNAL(clicked()), this, SLOT(set_fast_forward_mode()));
  connect(end_button, SIGNAL(clicked()), &firing_timer, SLOT(stop()));
  */

  set_time(start_time);
  old_manual_timer.set_time(start_time);
  set_timestep_length(manual_timer_timestep_length);
  redraw();
}

void AOTimer::update_time()
{
  manual_timer.perform_timestep();

  // Check for underflow/overflow

  // This one checks overflows, as the updated manual timer would appear to have a past time
  // compared to the manual timer a step ago (represented by old_manual_timer)
  if (manual_timer.get_timestep_length() > 0)
  {
      if (manual_timer.get_time().operator<(old_manual_timer.get_time()))
      {
        set_time(QTime(0, 0));
        firing_timer.stop();
        redraw();
        return;
      }
  }
  // This one checks underflows, as the updated manual timer would appear to have a future time
  // compared to the manual timer a step ago (represented by old_manual_timer)
  else if (manual_timer.get_timestep_length() < 0)
  {
      if (manual_timer.get_time().operator>(old_manual_timer.get_time()))
      {
        set_time(QTime(0, 0));
        firing_timer.stop();
        redraw();
        return;
      }
  }

  // If no overflow/underflow, now update the old manual timer so it matches the current timer
  // Redraw and return
  old_manual_timer.perform_timestep();
  redraw();
}

void AOTimer::set()
{
  set_time(start_time);
}

void AOTimer::resume()
{
  firing_timer.start(firing_timer_length);
}

void AOTimer::pause()
{
  firing_timer.stop();
}

void AOTimer::redraw()
{
  ui_timer_label->setText(manual_timer.get_time().toString("mm:ss.zzz"));
}

void AOTimer::set_time(QTime new_time)
{
  manual_timer.set_time(new_time);
  old_manual_timer.set_time(new_time);
  redraw();
}

void AOTimer::set_timestep_length(int new_timestep_length)
{
  manual_timer.set_timestep_length(new_timestep_length);
  old_manual_timer.set_timestep_length(new_timestep_length);
}

void AOTimer::set_firing_length(int new_firing_length)
{
  firing_timer_length = new_firing_length;
  //firing_timer.start(firing_timer_length);
}

void AOTimer::set_concentrate_mode()
{
  set_timestep_length(-12);
  set_firing_length(120);
}

void AOTimer::set_normal_mode()
{
  set_timestep_length(-12);
  set_firing_length(12);
}

void AOTimer::set_fast_forward_mode()
{
  set_timestep_length(-24);
  set_firing_length(12);
}
