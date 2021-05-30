#ifndef AOCHARBUTTON_H
#define AOCHARBUTTON_H

class AOApplication;
class AOImageDisplay;

#include <QPushButton>

class AOCharButton : public QPushButton
{
  Q_OBJECT

public:
  AOCharButton(QWidget *parent, AOApplication *p_ao_app, int x_pos, int y_pos);

  void reset();
  void set_taken();
  void set_image(QString p_character);

signals:
  void mouse_entered(AOCharButton *p_caller);
  void mouse_left();

protected:
  void enterEvent(QEvent *e);
  void leaveEvent(QEvent *e);

private:
  AOApplication *ao_app = nullptr;

  AOImageDisplay *ui_taken = nullptr;
};

#endif // AOCHARBUTTON_H
