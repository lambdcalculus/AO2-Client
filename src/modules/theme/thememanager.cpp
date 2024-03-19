#include "thememanager.h"


ThemeManager ThemeManager::s_Instance;

void ThemeManager::LoadTheme(QString theme_name)
{
  if(mRequiresReload)
  {
    mCurrentThemeReader.LoadTheme(theme_name);
    mRequiresReload = false;
  }
}

void ThemeManager::LoadGamemode(QString gamemode)
{
  mCurrentThemeReader.SetGamemode(gamemode);
}

void ThemeManager::TranslatePosition(QWidget *t_widget, int t_x, int t_y)
{
  int l_PositionX = static_cast<int>(t_x * mClientResize);
  int l_PositionY = static_cast<int>(t_y * mClientResize);

  t_widget->move(l_PositionX, l_PositionY);
}

void ThemeManager::ResizeWidget(QWidget *t_widget, int t_width, int t_height)
{
  int l_PositionWidth = static_cast<int>(t_width * mClientResize);
  int l_PositionHeight = static_cast<int>(t_height * mClientResize);

  t_widget->resize(l_PositionWidth, l_PositionHeight);
}

bool ThemeManager::getConfigBool(QString value)
{
  return mCurrentThemeReader.getConfigBool(value);
}

bool ThemeManager::getReloadPending()
{
  return mRequiresReload;
}

void ThemeManager::toggleReload()
{
  mRequiresReload = true;
}

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
