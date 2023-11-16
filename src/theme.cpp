#include "theme.h"

// std
#include <string>

// qt
#include <QDebug>
#include <QFontDatabase>
#include <QScreen>

// src
#include "aoapplication.h"
#include "commondefs.h"
#include "datatypes.h"
#include "drstickerviewer.h"
#include "drtextedit.h"
#include "drtheme.h"

void set_size_and_pos(QWidget *p_widget, QString p_identifier, QString p_ini_file, AOApplication *ao_app)
{
  pos_size_type design_ini_result = ao_app->get_element_dimensions(p_identifier, p_ini_file);

  if (design_ini_result.width < 0 || design_ini_result.height < 0)
  {
    qDebug() << "W: could not find" << p_identifier << "in " << p_ini_file;
    // Don't hide, as some widgets don't have a built-in way of reappearing again.
    p_widget->move(0, 0);
    p_widget->resize(0, 0);
  }
  else
  {
    p_widget->move(design_ini_result.x, design_ini_result.y);
    p_widget->resize(design_ini_result.width, design_ini_result.height);
  }
}

void set_text_alignment_or_default(QWidget *p_widget, QString p_identifier, QString p_ini_file, AOApplication *ao_app,
                                   std::string p_property, Qt::Alignment p_default_horizontal,
                                   Qt::Alignment p_default_vertical)
{
  const QStringList l_values =
      ao_app->current_theme->get_widget_font_string_setting(p_identifier, "align", p_ini_file, p_identifier + "_align").split(",", DR::SkipEmptyParts);
  if (!p_widget->property(p_property.c_str()).isValid())
    return;
  p_widget->setProperty(p_property.c_str(),
                        QVariant(QHash<QString, Qt::Alignment>{
                                     {"left", Qt::AlignLeft}, {"center", Qt::AlignHCenter}, {"right", Qt::AlignRight}}
                                     .value(l_values.value(0).trimmed().toLower(), p_default_horizontal) |
                                 QHash<QString, Qt::Alignment>{
                                     {"top", Qt::AlignTop}, {"center", Qt::AlignVCenter}, {"bottom", Qt::AlignBottom}}
                                     .value(l_values.value(1).trimmed().toLower(), p_default_vertical)));
}

void set_text_alignment(QWidget *p_widget, QString p_identifier, QString p_ini_file, AOApplication *ao_app)
{
  set_text_alignment_or_default(p_widget, p_identifier, p_ini_file, ao_app, "alignment", Qt::AlignLeft,
                                Qt::AlignVCenter);
}

void set_font(QWidget *p_widget, QString p_identifier, QString ini_file, AOApplication *ao_app)
{

  QString l_scene = "lobby";
  if(ini_file == COURTROOM_FONTS_INI) l_scene = "courtroom";


  QString class_name = p_widget->metaObject()->className();

  QFont l_font;
  // Font priority
  // 1. "font_" + p_identifier
  // 2. "font_default"
  // 3. System font
  QFontDatabase font_database;

  QString font_name = "";
  int f_weight = 1;
  bool is_bold = false;
  bool is_antialias = false;
  QColor l_font_color;

  if(ao_app->current_theme->m_jsonLoaded)
  {
    font_name = ao_app->current_theme->get_widget_font_name(p_identifier, l_scene);
    is_bold = ao_app->current_theme->get_widget_font_bool(p_identifier, l_scene, "bold");
    is_antialias = ao_app->current_theme->get_widget_font_bool(p_identifier, l_scene, "sharp");
    f_weight = ao_app->current_theme->get_widget_font_int(p_identifier, l_scene, "size");
    l_font_color = ao_app->current_theme->get_widget_font_color(p_identifier, l_scene);
  }
  else
  {
    font_name = ao_app->get_font_name("font_" + p_identifier, ini_file);
    f_weight = ao_app->get_font_property(p_identifier, ini_file);
    is_bold = ao_app->get_font_property(p_identifier + "_bold", ini_file) == 1;
    is_antialias = ao_app->get_font_property(p_identifier + "_sharp", ini_file) == 1;
    l_font_color = ao_app->get_color(p_identifier + "_color", ini_file);
  }

  if (!font_database.families().contains(font_name))
  {
    font_name = ao_app->get_font_name("font_default", ini_file);
  }
  if (!font_name.isEmpty())
  {
    l_font.setFamily(font_name);
  }


  l_font.setPointSize(f_weight);
  l_font.setBold(is_bold);

  if(is_antialias) l_font.setStyleStrategy(QFont::NoAntialias);
  else{l_font.setStyleStrategy(QFont::PreferDefault);}

  p_widget->setFont(l_font);

  QString style_sheet_string = class_name + " { " + "background-color: rgba(0, 0, 0, 0);\n" +
                               "color: " + l_font_color.name(QColor::HexArgb) + ";\n" + (is_bold ? "font: bold;" : "") +
                               "}";
  p_widget->setStyleSheet(style_sheet_string);
}

void set_drtextedit_font(DRTextEdit *p_widget, QString p_identifier, QString p_ini_file, AOApplication *ao_app)
{
  QString l_scene = "lobby";
  if(p_ini_file == COURTROOM_FONTS_INI) l_scene = "courtroom";

  set_font(p_widget, p_identifier, p_ini_file, ao_app);

  // Do outlines
  bool outline;

  if(ao_app->current_theme->m_jsonLoaded)
  {
    outline = ao_app->current_theme->get_widget_font_bool(p_identifier, l_scene, "outline");
  }
  else
  {
    outline = (ao_app->get_font_property(p_identifier + "_outline", p_ini_file) == 1);
  }
  p_widget->set_outline(outline);

  // alignment
  set_text_alignment_or_default(p_widget, p_identifier, p_ini_file, ao_app, "text_alignment", Qt::AlignLeft,
                                Qt::AlignTop);
}

/**
 * @brief set_stylesheet
 * @param p_widget The widget to apply the stylesheet to
 * @param p_identifier The identifier within the stylesheet
 * @param p_ini_file The stylesheet file (glorified ini)
 * @param ao_app AOApplication shenanigan
 * @return Return true if any data (even if invalid) was applied to the widget
 */
bool set_stylesheet(QWidget *p_widget, QString p_identifier, QString p_ini_file, AOApplication *ao_app)
{
  const QString p_style = ao_app->get_stylesheet(p_identifier, p_ini_file);
  p_widget->setStyleSheet(p_style);
  return !p_style.isEmpty();
}

/**
 * @brief Center the widget on the screen it's resting in.
 * @param widget The widget to center.
 */
void center_widget_to_screen(QWidget *p_widget)
{
  if (!p_widget || p_widget->parentWidget())
    return;

  QScreen *screen = QApplication::screenAt(p_widget->pos());
  if (screen == nullptr)
    return;
  QRect screen_geometry = screen->geometry();
  int x = (screen_geometry.width() - p_widget->width()) / 2;
  int y = (screen_geometry.height() - p_widget->height()) / 2;
  p_widget->move(x, y);
}

void set_sticker_play_once(DRStickerViewer *p_sticker, QString p_identifier, QString p_ini_file, AOApplication *ao_app)
{
  const bool l_play_once = ao_app->read_theme_ini_bool(p_identifier + "_play_once", p_ini_file);
  p_sticker->set_play_once(l_play_once);
  if (!p_sticker->is_running())
  {
    p_sticker->start();
  }
}
