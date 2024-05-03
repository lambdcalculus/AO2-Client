#include "debugmenuui.h"
#include "ui_debugmenuui.h"

DebugMenuUI::DebugMenuUI(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::DebugMenuUI)
{
  ui->setupUi(this);
}

DebugMenuUI::~DebugMenuUI()
{
  delete ui;
}
