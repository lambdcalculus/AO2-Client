#ifndef CHARACTERSELECTWIDGET_H
#define CHARACTERSELECTWIDGET_H

#include <aoapplication.h>
#include <QObject>
#include <QWidget>

class CharacterSelectWidget : public QWidget
{
  Q_OBJECT
public:
  CharacterSelectWidget(QWidget *parent, AOApplication *p_ao_app);
signals:
protected:
  void wheelEvent(QWheelEvent *event) override;

private:
  AOApplication *ao_app = nullptr;

};

#endif // CHARACTERSELECTWIDGET_H
