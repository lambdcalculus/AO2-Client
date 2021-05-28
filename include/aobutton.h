#ifndef AOBUTTON_H
#define AOBUTTON_H

#include "aoapplication.h"

#include <QPushButton>

class AOButton : public QPushButton
{
  Q_OBJECT

public:
  AOButton(QWidget *parent, AOApplication *p_ao_app);

  QString get_image();
  void set_image(QString p_image);

private:
  AOApplication *ao_app = nullptr;
  QString image_path;
};

#endif // AOBUTTON_H
