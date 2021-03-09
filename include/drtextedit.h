#ifndef DRTEXTEDIT_H
#define DRTEXTEDIT_H

#include <QTextEdit>
#include <QWidget>

class DRTextEdit : public QTextEdit
{
public:
  DRTextEdit(QWidget *p_parent);

  void setOutline(bool outline);
};

#endif // DRTEXTEDIT_H
