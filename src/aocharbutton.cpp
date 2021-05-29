#include "aocharbutton.h"

#include "aoapplication.h"
#include "aoimagedisplay.h"
#include "file_functions.h"

#include <QFile>

AOCharButton::AOCharButton(QWidget *parent, AOApplication *p_ao_app, int x_pos, int y_pos) : QPushButton(parent)
{
  ao_app = p_ao_app;

  this->resize(60, 60);
  this->move(x_pos, y_pos);

  ui_taken = new AOImageDisplay(this, ao_app);
  ui_taken->resize(60, 60);
  ui_taken->set_image("char_taken.png");
  ui_taken->setAttribute(Qt::WA_TransparentForMouseEvents);
  ui_taken->hide();
}

void AOCharButton::reset()
{
  ui_taken->hide();
}

void AOCharButton::set_taken()
{
  ui_taken->show();
}

void AOCharButton::set_image(QString p_character)
{
  const QString l_image = ao_app->get_character_path(p_character, "char_icon.png");
  const bool l_file_exist = file_exists(l_image);
  setStyleSheet(l_file_exist ? QString("border-image: url(\"%1\");").arg(l_image) : nullptr);
  setText(l_file_exist ? nullptr : p_character);
}

void AOCharButton::enterEvent(QEvent *e)
{
  setFlat(false);
  QPushButton::enterEvent(e);
  Q_EMIT mouse_entered(this);
}

void AOCharButton::leaveEvent(QEvent *e)
{
  QPushButton::leaveEvent(e);
  Q_EMIT mouse_left();
}
