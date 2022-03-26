#ifndef AONOTEAREA_H
#define AONOTEAREA_H

#include "aoimagedisplay.h"

class AOApplication;
class AOButton;

class QVBoxLayout;

class AONoteArea : public AOImageDisplay
{
  Q_OBJECT

public:
  AOButton *add_button = nullptr;
  QVBoxLayout *m_layout = nullptr;

  AONoteArea(QWidget *p_parent, AOApplication *p_ao_app);

private:
  AOApplication *ao_app = nullptr;

  void set_layout();
};

#endif // AONOTEAREA_H
