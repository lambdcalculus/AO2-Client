#ifndef DRTEXTEDIT_H
#define DRTEXTEDIT_H

#include <QTextEdit>

class DRTextEdit : public QTextEdit
{
  Q_OBJECT

public:
  DRTextEdit(QWidget *p_parent);

  Qt::Alignment get_vertical_alignment();
  Qt::Alignment get_horizontal_alignment();

  void set_outline(bool enabled);
  void set_auto_align(bool enabled);
  void set_vertical_alignment(Qt::Alignment p_align);
  void set_horizontal_alignment(Qt::Alignment p_align);

private:
  Qt::Alignment m_valign = Qt::AlignTop;
  Qt::Alignment m_halign = Qt::AlignLeft;
  bool has_outline = false;
  bool is_auto_align = true;
  enum class Status
  {
    Done,
    InProgress,
  };
  Status m_status = Status::Done;
  int m_current_document_blocks = 0;
  int m_current_document_height = 0;

  void refresh_horizontal_alignment();
  void refresh_vertical_alignment();

private slots:
  void on_text_changed();
};

#endif // DRTEXTEDIT_H
