#ifndef TABGROUPINGWIDGET_H
#define TABGROUPINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <aoimagedisplay.h>

class TabGroupingWidget : public QWidget
{
  Q_OBJECT
public:
  TabGroupingWidget(QWidget *parent = nullptr);
  void setBackgroundImage(QString t_name);

  AOImageDisplay *w_BackgroundImage = nullptr;

};

#endif // TABGROUPINGWIDGET_H
