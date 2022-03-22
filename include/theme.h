#pragma once

// src
class AOApplication;
class DRTextEdit;

// qt
class QLineEdit;
class QString;
class QWidget;

void set_size_and_pos(QWidget *widget, QString identifier, QString ini_file, AOApplication *ao_app);
void set_text_alignment(QWidget *widget, QString identifier, QString ini_file, AOApplication *ao_app);
void set_font(QWidget *widget, QString identifier, QString ini_file, AOApplication *ao_app);
void set_drtextedit_font(DRTextEdit *widget, QString identifier, QString ini_file, AOApplication *ao_app);
bool set_stylesheet(QWidget *widget, QString identifier, QString ini_file, AOApplication *ao_app);
void center_widget_to_screen(QWidget *widget);
