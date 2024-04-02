#ifndef VIEWPORTOVERLAY_H
#define VIEWPORTOVERLAY_H

#include <QObject>
#include <QWidget>

class ViewportOverlay : public QWidget
{
  Q_OBJECT
public:
  explicit ViewportOverlay(QWidget *parent = nullptr);

signals:

};

#endif // VIEWPORTOVERLAY_H
