#ifndef DEBUGMENUUI_H
#define DEBUGMENUUI_H

#include <QMainWindow>

namespace Ui {
class DebugMenuUI;
}

class DebugMenuUI : public QMainWindow
{
  Q_OBJECT

public:
  explicit DebugMenuUI(QWidget *parent = nullptr);
  ~DebugMenuUI();

private:
  Ui::DebugMenuUI *ui;
};

#endif // DEBUGMENUUI_H
