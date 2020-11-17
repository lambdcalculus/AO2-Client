#ifndef AONOTEAREA_HPP
#define AONOTEAREA_HPP

#include <QLabel>
#include <QListWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>
#include <aobutton.h>
#include <aoimagedisplay.h>
#include <aonotepicker.h>

#include "aoapplication.h"

class AONoteArea : public AOImageDisplay
{
  Q_OBJECT

public:
  AONoteArea(QWidget *p_parent, AOApplication *p_ao_app);

  AOButton *add_button;
  QVBoxLayout *m_layout;

private:
  AOApplication *ao_app;
  void set_layout();
};

#endif // AONOTEAREA_HPP
