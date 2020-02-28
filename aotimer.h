#ifndef AOTIMER_H
#define AOTIMER_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <aolabel.hpp>
#include <aobutton.h>

class Chronometer {
    QElapsedTimer t;
  public:
    void restart() { t.restart(); }
    QTime getTime() { return QTime(0,0).addMSecs(t.elapsed()); }
};

class AOTimer : public QWidget
{
  Q_OBJECT

public:
  AOTimer(QWidget* p_parent, AOApplication *p_ao_app);

private:
  AOApplication *ao_app = nullptr;
  AOLabel *ui_timer_label;

  Chronometer c;
  QTimer t;

public slots:
  void updateTime();
  void start();
};


#endif // AOTIMER_H


/*
 * #ifndef AOTEXTEDIT_H
#define AOTEXTEDIT_H

#include <QPlainTextEdit>

class AOTextEdit : public QPlainTextEdit
{
  Q_OBJECT
public:
  AOTextEdit(QWidget *parent);

protected:
  void mouseDoubleClickEvent(QMouseEvent *e);

signals:
  void double_clicked();

private slots:
  void on_enter_pressed();

};

#endif // AOTEXTEDIT_H
*/
