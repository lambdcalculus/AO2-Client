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
  void setHorizontalAlignment(Qt::Alignment);

  bool outline();
  Qt::Alignment verticalAlignment();
  Qt::Alignment horizontalAlignment();

private:
  bool _outline = false;
  Qt::Alignment _verticalAlignment = Qt::AlignTop;
  Qt::Alignment _horizontalAlignment = Qt::AlignLeft;
  int previous_height = 0;

  bool processing_change = false;
  void _onTextChanged();

private slots:
  void onTextChanged();
};

#endif // DRTEXTEDIT_H
