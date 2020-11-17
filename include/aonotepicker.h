#ifndef AONOTEPICKER_HPP
#define AONOTEPICKER_HPP

#include "aobutton.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

class AONotePicker : public QLabel
{
  Q_OBJECT

public:
  AONotePicker(QWidget *p_parent, AOApplication *p_ao_app);

  QLineEdit *m_line = nullptr;
  AOButton *m_button = nullptr;
  AOButton *m_delete_button = nullptr;
  AOButton *m_hover = nullptr;
  QHBoxLayout *m_layout = nullptr;
  QString real_file = "";

private:
  AOApplication *ao_app = nullptr;
};

#endif // AONOTEPICKER_HPP
