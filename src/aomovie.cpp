#include "aomovie.h"

#include "courtroom.h"
#include "file_functions.h"
#include "misc_functions.h"

AOMovie::AOMovie(QWidget *p_parent, AOApplication *p_ao_app)
    : QLabel(p_parent)
{
  ao_app = p_ao_app;

  m_movie = new QMovie();

  this->setMovie(m_movie);

  connect(m_movie, SIGNAL(frameChanged(int)), this, SLOT(frame_change(int)));
}

AOMovie::~AOMovie()
{
  delete m_movie;
}

void AOMovie::set_play_once(bool p_play_once)
{
  play_once = p_play_once;
}

void AOMovie::play(QString p_file, QString p_char)
{
  m_movie->stop();
  QVector<QString> f_vec;
  QString file_path = "";

  // Remove ! at the beginning of p_file if needed
  // This is an indicator that the file is not selectable in the current theme
  // (variant) but is still usable by other people
  if (p_file.length() > 0 && p_file.at(0) == "!")
    p_file = p_file.remove(0, 1);

  QString custom_path;
  if (p_file == "custom")
    custom_path = ao_app->get_character_path(p_char, p_file);
  else
    custom_path = ao_app->get_character_path(p_char, p_file + "_bubble");

  QStringList f_paths{
      custom_path,
      ao_app->get_character_path(p_char, "overlay/" + p_file),
      ao_app->get_theme_variant_path(p_file),
      ao_app->get_theme_path(p_file),
      ao_app->get_default_theme_path(p_file),
      ao_app->get_theme_variant_path("placeholder"),
      ao_app->get_theme_path("placeholder"),
      ao_app->get_default_theme_path("placeholder"),
  };

  for (auto &f_file : f_paths)
  {
    bool found = false;
    for (auto &ext : decltype(f_vec){".webp", ".apng", ".gif", ".png"})
    {
      QString fullPath = ao_app->get_case_sensitive_path(f_file + ext);
      found = file_exists(fullPath);
      if (found)
      {
        file_path = fullPath;
        break;
      }
    }

    if (found)
      break;
  }

  qDebug() << file_path;
  qDebug() << "playing" << file_path;
  m_movie->setFileName(file_path);

  this->show();
  m_movie->setScaledSize(this->size());
  m_movie->start();
}

void AOMovie::play_interjection(QString p_char_name,
                                QString p_interjection_name)
{
  m_movie->stop();

  QString chr_interjection;
  {
    QString interjection_suffix = "_bubble";

    // FIXME there is no reason custom shouldn't have a suffix
    if (p_interjection_name.toLower() == "custom")
    {
      interjection_suffix.clear();
    }

    chr_interjection = ao_app->get_character_path(
        p_char_name, p_interjection_name + interjection_suffix);
  }

  QStringList possible_paths{
      chr_interjection,
      ao_app->get_theme_variant_path(p_interjection_name),
      ao_app->get_theme_path(p_interjection_name),
      ao_app->get_default_theme_path(p_interjection_name),
  };

  QString interjection_filepath;
  for (QString &i_path : possible_paths)
  {
    bool skip = false;

    for (QString &i_ext : QStringList{".webp", ".apng", ".gif"})
    {
      QString fullpath = ao_app->get_case_sensitive_path(i_path + i_ext);
      if (file_exists(fullpath))
      {
        skip = true;
        interjection_filepath = fullpath;
        break;
      }
    }

    if (skip)
    {
      break;
    }
  }

  if (interjection_filepath.isEmpty())
  {
    emit done();
    return;
  }

  qDebug() << "playing interjection"
           << (p_interjection_name.isEmpty() ? "(none)" : p_interjection_name)
           << "for character"
           << (p_char_name.isEmpty() ? "(none)" : p_char_name) << "at"
           << (interjection_filepath.isEmpty() ? "(not found)" : interjection_filepath);
  m_movie->setFileName(interjection_filepath);
  this->show();
  m_movie->setScaledSize(this->size());
  m_movie->start();
}

void AOMovie::stop()
{
  m_movie->stop();
  this->hide();
}

void AOMovie::frame_change(int n_frame)
{
  if (n_frame == (m_movie->frameCount() - 1) && play_once)
  {
    // we need this or else the last frame wont show
    delay(m_movie->nextFrameDelay());

    this->stop();

    // signal connected to courtroom object, let it figure out what to do
    emit done();
  }
}

void AOMovie::combo_resize(int w, int h)
{
  QSize f_size(w, h);
  this->resize(f_size);
  m_movie->setScaledSize(f_size);
}
