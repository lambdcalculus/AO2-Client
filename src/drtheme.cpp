#include "drtheme.h"
#include "commondefs.h"
#include "file_functions.h"
#include "qjsonobject.h"
#include <QJsonArray>

#include <qfile.h>

DRTheme::DRTheme(AOApplication *p_ao_app)
{
  ao_app = p_ao_app;
}

void DRTheme::LoadJson()
{

  const QString l_json_path = ao_app->find_theme_asset_path(THEME_JSON);

  if(!file_exists(l_json_path))
  {
    m_currentThemeString = "";
    m_jsonLoaded = false;
    return;
  }

  QFile json_file(l_json_path); json_file.open(QIODevice::ReadOnly | QIODevice::Text);
  m_currentThemeString = json_file.readAll();
  json_file.close();

  //Create a json document from the loaded text.
  m_currentThemeDocument = QJsonDocument::fromJson(m_currentThemeString.toUtf8());
  m_currentThemeObject = m_currentThemeDocument.object();

  m_jsonLoaded = true;

}

bool DRTheme::read_config_bool(QString p_setting_name)
{
  if(!m_jsonLoaded)
  {
    return ao_app->read_theme_ini_bool(p_setting_name, COURTROOM_CONFIG_INI);
  }

  QJsonValue value = m_currentThemeObject.value(QString("config"));
  QJsonObject item = value.toObject();
  bool return_value = item[p_setting_name].toBool();

  return return_value;
}

QVector<QStringList>DRTheme::get_highlight_characters()
{

  QVector<QStringList> f_vec;

  QJsonValue value = m_currentThemeObject.value(QString("config"));
  QJsonObject item = value.toObject();
  QJsonArray array = item["ic_text_colors"].toObject()["highlights"].toArray();

  for(QJsonValueRef i : array)
  {
    QJsonObject arrayobject = i.toObject();

    f_vec.append({arrayobject["chars"].toString(), arrayobject["color"].toString(), arrayobject["keep_characters"].toBool() ? "1" : "0"});
  }

  return f_vec;
}

pos_size_type DRTheme::get_element_dimensions(QString p_identifier, QString p_scene)
{
  pos_size_type return_value;
  return_value.x = 0;
  return_value.y = 0;
  return_value.width = -1;
  return_value.height = -1;


  QJsonValue value = m_currentThemeObject.value(QString(p_scene));
  QJsonObject item = value.toObject();
  QJsonObject element_position = item[p_identifier].toObject();

  if(!element_position.contains("position"))
  {
    return return_value;
  }

  return_value.x = element_position["position"].toObject()["x"].toInt();
  return_value.y = element_position["position"].toObject()["y"].toInt();
  return_value.width = element_position["position"].toObject()["width"].toInt();
  return_value.height = element_position["position"].toObject()["height"].toInt();

  return return_value;
}

QString DRTheme::get_widget_image(QString p_identifier, QString p_fallback, QString p_scene)
{
  if(!m_jsonLoaded)
  {
    return p_fallback;
  }

  QJsonValue value = m_currentThemeObject.value(QString(p_scene));
  QJsonObject item = value.toObject();
  QJsonObject element_image = item[p_identifier].toObject();

  qDebug() << element_image;
  {
    return p_fallback;
  }

  return element_image["image"].toString();

};
