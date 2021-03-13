#ifndef DRTEXTEDIT_H
#define DRTEXTEDIT_H

#include <QTextEdit>
#include <QWidget>

class DRTextEdit : public QTextEdit
{
  Q_OBJECT

public:
  DRTextEdit(QWidget *p_parent);

  bool get_outline();
  Qt::Alignment get_vertical_alignment();
  Qt::Alignment get_horizontal_alignment();

  void set_outline(bool p_outline);
  void set_vertical_alignment(Qt::Alignment p_align);
  void set_horizontal_alignment(Qt::Alignment p_align);

private:
  bool m_outline = false;
  Qt::Alignment m_valign = Qt::AlignTop;
  Qt::Alignment m_halign = Qt::AlignLeft;

  enum class Status
  {
    Done,
    InProgress,
  };
  Status m_status = Status::Done;

private slots:
  void on_text_changed();
};

#endif // DRTEXTEDIT_H
