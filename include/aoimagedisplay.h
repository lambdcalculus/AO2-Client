#ifndef AOIMAGEDISPLAY_H
#define AOIMAGEDISPLAY_H

#include "aoapplication.h"
#include "aopixmap.h"

#include <QLabel>

// This class represents a static theme-dependent image
class AOImageDisplay : public QLabel
{
public:
  AOImageDisplay(QWidget *parent, AOApplication *p_ao_app);

  void set_image(QString p_image);
  void set_image_from_path(QString p_path);
  void set_size_and_pos(QString identifier);

  AOApplication *ao_app = nullptr;
  QString image_path;
};

#endif // AOIMAGEDISPLAY_H
