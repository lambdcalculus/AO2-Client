#include "aoscene.h"

#include "courtroom.h"

#include "file_functions.h"

// core
#include <QDebug>

// gui
#include <QMovie>

AOScene::AOScene(QWidget *parent, AOApplication *p_ao_app) : QLabel(parent)
{
  m_parent = parent;
  m_movie = new QMovie(this);
  ao_app = p_ao_app;
}

void AOScene::set_image(QString p_image)
{
  QString animated_background_path = ao_app->get_background_path() + p_image;
  QString background_path = ao_app->get_background_path() + p_image + ".png";
  QString default_path = ao_app->get_default_background_path() + p_image;

  for (auto& ext : QVector<QString>{".apng", ".gif"})
  {
    QString full_path = animated_background_path + ext;
    if (file_exists(full_path))
    {
      animated_background_path = full_path;
      break;
    }
  }

  QPixmap animated_background(animated_background_path);
  QPixmap background(background_path);
  QPixmap default_bg(default_path);

  int w = this->width();
  int h = this->height();

  // remove movie
  this->clear();
  this->setMovie(nullptr);
  // stop current movie
  m_movie->stop();
  m_movie->setFileName(animated_background_path);
  m_movie->setScaledSize(QSize(w, h));

  if (m_movie->isValid())
  {
    this->setMovie(m_movie);
    m_movie->start();
  }
  else if (file_exists(background_path))
  {
    this->setPixmap(background.scaled(w, h));
  }
  else
  {
    this->setPixmap(default_bg.scaled(w, h));
  }
}
