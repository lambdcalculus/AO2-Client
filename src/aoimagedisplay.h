#ifndef AOIMAGEDISPLAY_H
#define AOIMAGEDISPLAY_H

class AOApplication;

#include <QLabel>

class AOImageDisplay : public QLabel
{
public:
  AOImageDisplay(QWidget *parent, AOApplication *p_ao_app);

  QString get_image();
  void set_image(QString p_image);
  void set_theme_image(QString p_image);
  void set_chatbox_image(QString p_image);

private:
  AOApplication *ao_app = nullptr;

  QString m_image;
};

#endif // AOIMAGEDISPLAY_H
