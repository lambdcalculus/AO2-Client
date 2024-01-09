#ifndef DROEMOTEBUTTONS_H
#define DROEMOTEBUTTONS_H

#include <AOApplication.h>
#include <QObject>
#include <QWidget>

class DROEmoteButtons : public QWidget
{
  Q_OBJECT
public:
  explicit DROEmoteButtons(QWidget *parent, AOApplication *p_ao_app);
signals:
protected:
  void wheelEvent(QWheelEvent *event) override;

private:
  AOApplication *ao_app = nullptr;
};

#endif // DROEMOTEBUTTONS_H
