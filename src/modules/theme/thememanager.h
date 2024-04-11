#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <AOButton.h>
#include <QHash>

#include <modules/theme/widgets/dro_combo_box.h>
#include <modules/theme/widgets/dro_line_edit.h>
#include "modules/json/theme_reader.h"



class ThemeManager
{
public:
  ThemeManager(const ThemeManager&) = delete;

  static ThemeManager& get()
  {
    return s_Instance;
  }

  void LoadTheme(QString theme_name);
  void LoadGamemode(QString gamemode);

  void TranslatePosition(QWidget *t_widget, int x, int y);
  void ResizeWidget(QWidget *t_widget, int t_width, int t_height);

  //Widgets
  void SetWidgetNames(QHash<QString, QWidget *> t_WidgetNames);
  void addWidgetName(QString t_widgetName, QWidget *t_widget);

  //Tabs
  void ToggleTab(QString tabName);


  //Data Management
  bool getConfigBool(QString value);
  bool getReloadPending();
  void toggleReload();

  //Widget Management
  pos_size_type resizePosition(pos_size_type t_position, double t_scale);

  void setResize(double size);
  double getResize();
  void setViewporResize(double size);
  double getViewporResize();

  void addButton(QString name, AOButton* button);
  void refreshButtons();

  void addLineEdit(QString name, DROLineEdit* lineEdit);
  void addComboBox(QString name, DROComboBox* lineEdit);
  void refreshLineEdit();
  void refreshComboBox();

  QWidget *GetWidget(QString name);
  AOButton *GetButton(QString t_name);

  ThemeReader mCurrentThemeReader = ThemeReader();

private:
  ThemeManager() {}
  static ThemeManager s_Instance;

  double mClientResize = 1;

  double mViewportResize = 1;


  QHash<QString, QWidget *> m_WidgetNames = {};
  QHash<QString, AOButton*> mButtonWidgets { };
  QHash<QString, DROLineEdit*> mLineEditWidgets { };
  QHash<QString, DROComboBox*> mComboBoxWidgets { };

  bool mRequiresReload = true;
};

#endif // THEMEMANAGER_H
