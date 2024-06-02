#ifndef DROEMOTEBUTTONS_H
#define DROEMOTEBUTTONS_H

#include <QObject>
#include <QWidget>
#include <aoapplication.h>

class DROEmoteButtons : public QWidget
{
  Q_OBJECT
public:
  explicit DROEmoteButtons(QWidget *parent);
signals:
protected:
  void wheelEvent(QWheelEvent *event) override;

};

#endif // DROEMOTEBUTTONS_H
