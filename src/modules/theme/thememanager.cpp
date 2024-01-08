#include "thememanager.h"


ThemeManager ThemeManager::s_Instance;

void ThemeManager::setResize(double size)
{
  mClientResize = size;
}

double ThemeManager::getResize()
{
  return mClientResize;
}

void ThemeManager::addButton(QString name, AOButton *button)
{
  mButtonWidgets[name] = button;
}

void ThemeManager::addLineEdit(QString name, DROLineEdit* lineEdit)
{
  mLineEditWidgets[name] = lineEdit;
}

void ThemeManager::refreshButtons()
{
  for(AOButton* button : mButtonWidgets)
  {
    button->set_theme_image();
    button->refresh_position();
    button->show();
  }
}

void ThemeManager::refreshLineEdit()
{
  for(DROLineEdit* lineEdit : mLineEditWidgets)
  {
    lineEdit->refreshPosition();
    lineEdit->refreshCSS();
    lineEdit->show();
  }
}

void ThemeManager::addComboBox(QString name, DROComboBox *comboBox)
{
  mComboBoxWidgets[name] = comboBox;
}

void ThemeManager::refreshComboBox()
{
  for(DROComboBox* comboBox : mComboBoxWidgets)
  {
    comboBox->refreshPosition();
    comboBox->refreshCSS();
    comboBox->show();
  }
}
