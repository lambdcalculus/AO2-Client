#ifndef DRPLAYERLISTENTRY_H
#define DRPLAYERLISTENTRY_H
class AOApplication;

#include "aoimagedisplay.h"
#include "aolabel.h"

#include <QWidget>

class DrPlayerListEntry : public QWidget
{
public:
    DrPlayerListEntry(QWidget *p_parent, AOApplication *p_ao_app, int p_x, int p_y);
    void set_character(QString p_character);
    void set_name(QString showname);
    void set_reason(QString p_reason);
    AOImageDisplay *ui_user_image = nullptr;
    AOLabel *ui_showname = nullptr;
    AOLabel *m_prompt = nullptr;


    int m_entrywidth = 0;

private:
  AOApplication *ao_app = nullptr;

  int m_index = 0;
  QImage m_texture;
  QString m_showname;
  QString m_character;

};

#endif // DRPLAYERLISTENTRY_H
