#include "theme.h"

// src
#include "aoapplication.h"
#include "datatypes.h"
#include "drtextedit.h"

// qt
#include <QDebug>
#include <QFontDatabase>

void set_size_and_pos(QWidget *p_widget, QString p_identifier, QString p_ini_file, AOApplication *ao_app)
{
  pos_size_type design_ini_result = ao_app->get_element_dimensions(p_identifier, p_ini_file);

  if (design_ini_result.width < 0 || design_ini_result.height < 0)
  {
    qDebug() << "W: could not find \"" << p_identifier << "\" in " << p_ini_file;
    p_widget->hide();
  }
  else
  {
    p_widget->move(design_ini_result.x, design_ini_result.y);
    p_widget->resize(design_ini_result.width, design_ini_result.height);
  }
}

void set_text_alignment(QWidget *p_widget, QString p_identifier, QString p_ini_file, AOApplication *ao_app)
{
  const QStringList l_values =
      ao_app->read_theme_ini(p_identifier + "_align", p_ini_file).split(",", DR::SkipEmptyParts);
  p_widget->setProperty("alignment",
                        QVariant(QHash<QString, Qt::AlignmentFlag>{
                                     {"left", Qt::AlignLeft}, {"center", Qt::AlignHCenter}, {"right", Qt::AlignRight}}
                                     .value(l_values.value(0).trimmed().toLower(), Qt::AlignLeft) |
                                 QHash<QString, Qt::AlignmentFlag>{
                                     {"top", Qt::AlignTop}, {"center", Qt::AlignVCenter}, {"bottom", Qt::AlignBottom}}
                                     .value(l_values.value(1).trimmed().toLower(), Qt::AlignVCenter)));
}

void set_font(QWidget *p_widget, QString p_identifier, QString ini_file, AOApplication *ao_app)
{
  QString class_name = p_widget->metaObject()->className();

  int f_weight = ao_app->get_font_property(p_identifier, ini_file);

  // Font priority
  // 1. "font_" + p_identifier
  // 2. "font_default"
  // 3. System font
  QFontDatabase font_database;
  QString font_name = ao_app->get_font_name("font_" + p_identifier, ini_file);
  if (!font_database.families().contains(font_name))
  {
    font_name = ao_app->get_font_name("font_default", ini_file);
  }
  p_widget->setFont(QFont(font_name, f_weight));

  QString font_color = ao_app->read_theme_ini(p_identifier + "_color", ini_file);
  if (font_color.isEmpty())
    font_color = "255, 255, 255";
  QString color = "rgba(" + font_color + ", 255)";

  int bold = ao_app->get_font_property(p_identifier + "_bold", ini_file);
  QString is_bold = (bold == 1 ? "bold" : "");

  QString style_sheet_string = class_name + " { " + "background-color: rgba(0, 0, 0, 0);\n" + "color: " + color +
                               ";\n" + "font: " + is_bold + ";" + " }";
  p_widget->setStyleSheet(style_sheet_string);
}

void set_drtextedit_font(DRTextEdit *p_widget, QString p_identifier, QString ini_file, AOApplication *ao_app)
{
  set_font(p_widget, p_identifier, ini_file, ao_app);

  // Do outlines
  bool outline = (ao_app->get_font_property(p_identifier + "_outline", ini_file) == 1);
  p_widget->set_outline(outline);

  // Do horizontal alignments
  int raw_halign = ao_app->get_font_property(p_identifier + "_halign", ini_file);
  switch (raw_halign)
  {
  default:
    qWarning() << "Unknown horizontal alignment for " + p_identifier + ". Assuming Left.";
    [[fallthrough]];
  case DR::Left:
    p_widget->set_horizontal_alignment(Qt::AlignLeft);
    break;
  case DR::Middle:
    p_widget->set_horizontal_alignment(Qt::AlignHCenter);
    break;
  case DR::Right:
    p_widget->set_horizontal_alignment(Qt::AlignRight);
    break;
  }

  // Do vertical alignments
  int raw_valign = ao_app->get_font_property(p_identifier + "_valign", ini_file);
  Qt::Alignment valignment;
  switch (raw_valign)
  {
  default:
    qWarning() << "Unknown vertical alignment for" << p_identifier << ":" << raw_valign << "Assuming Top.";
    [[fallthrough]];
  case DR::Top:
    valignment = Qt::AlignTop;
    break;
  case DR::Middle:
    valignment = Qt::AlignVCenter;
    break;
  case DR::Bottom:
    valignment = Qt::AlignBottom;
    break;
  }
  p_widget->set_vertical_alignment(valignment);
}
