#include "aobutton.h"

#include "aoapplication.h"
#include "debug_functions.h"
#include "file_functions.h"

#include <QDebug>

AOButton::AOButton(QWidget *parent, AOApplication *p_ao_app) : QPushButton(parent)
{
  ao_app = p_ao_app;
}

QString AOButton::get_image()
{
  return m_image;
}

bool AOButton::has_image()
{
  return (!m_image.isEmpty());
}

void AOButton::set_image(QString p_image)
{
  m_image_stem = p_image;
  m_image = ao_app->find_theme_asset_path(p_image);

  // Get the path of the found image without the extension
  const QString l_image_name = p_image.left(p_image.lastIndexOf(QChar('.')));
  QString l_hover_image = ao_app->find_theme_asset_path(l_image_name + "_hover.png");
  if (l_hover_image.isEmpty())
  {
    l_hover_image = m_image;
  }

  setStyleSheet("QPushButton {border-image:url(\"" + m_image +
                "\");}"
                "QPushButton:hover {border-image:url(\"" +
                l_hover_image + "\");}");
  setText(m_image.isEmpty() ? m_text : nullptr);
}

void AOButton::set_image_and_text(QString p_image, QString p_text)
{
  m_text = p_text;
  set_image(p_image);
}

void AOButton::refresh_image()
{
  set_image(m_image_stem);
}
