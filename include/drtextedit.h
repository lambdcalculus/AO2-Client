#ifndef DRTEXTEDIT_H
#define DRTEXTEDIT_H

#include <QTextEdit>
#include <QWidget>

class DRTextEdit : public QTextEdit
{
  Q_OBJECT

public:
  DRTextEdit(QWidget *p_parent);

  void setOutline(bool outline);
  int heightSpan();

private:
  Qt::Alignment verticalAlignment = Qt::AlignTop;
  int previous_height = 0;

private slots:
  void onTextChanged();
};

#endif // DRTEXTEDIT_H
