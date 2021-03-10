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
  void setVerticalAlignment(Qt::Alignment);

  bool outline();
  Qt::Alignment verticalAlignment();

private:
  bool _outline = false;
  Qt::Alignment _verticalAlignment = Qt::AlignTop;
  int previous_height = 0;

private slots:
  void onTextChanged();
};

#endif // DRTEXTEDIT_H
