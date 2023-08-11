#ifndef DRTHEME_H
#define DRTHEME_H

#include "qjsonobject.h"
#include <AOApplication.h>
#include <qjsondocument.h>
#include <qstring.h>



class DRTheme
{
public:
  DRTheme(AOApplication *p_ao_app);

  QString m_currentThemeString;
  QJsonDocument m_currentThemeDocument;
  QJsonObject m_currentThemeObject;

  void LoadJson();
  bool read_config_bool(QString p_setting_name);
  QVector<QStringList> get_highlight_characters();
  bool m_jsonLoaded = false;

private:
  AOApplication *ao_app = nullptr;
};

#endif // DRTHEME_H
