#include "tabgroupingwidget.h"
#include "qapplication.h"
#include "qcoreevent.h"
#include "qdebug.h"
#include "aoapplication.h"
#include <QMouseEvent>


TabGroupingWidget::TabGroupingWidget(QWidget *parent) : QWidget(parent)
{
  w_BackgroundImage = new AOImageDisplay(this, AOApplication::getInstance());
}

void TabGroupingWidget::setBackgroundImage(QString t_name)
{
  w_BackgroundImage->move(0, 0);
  w_BackgroundImage->resize(this->width(), this->height());
  w_BackgroundImage->set_theme_image(t_name + "_panel.png");
  w_BackgroundImage->show();
}
