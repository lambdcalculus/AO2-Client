#include "aomovie.h"

#include "aoapplication.h"
#include "aopixmap.h"
#include "file_functions.h"
#include "misc_functions.h"

#include <QDebug>

AOMovie::AOMovie(QWidget *p_parent, AOApplication *p_ao_app) : QLabel(p_parent)
{
  ao_app = p_ao_app;

  m_movie = new QMovie();

  connect(m_movie, SIGNAL(frameChanged(int)), this, SLOT(frame_change(int)));
  connect(m_movie, SIGNAL(stateChanged(QMovie::MovieState)), this, SLOT(handle_state(QMovie::MovieState)));
}

AOMovie::~AOMovie()
{
  delete m_movie;
}

bool AOMovie::is_play_once()
{
  return m_play_once;
}

void AOMovie::set_play_once(bool p_play_once)
{
  m_play_once = p_play_once;
}

bool AOMovie::is_hide_on_done()
{
  return m_hide_on_done;
}

void AOMovie::set_hide_on_done(bool p_hide_on_done)
{
  m_hide_on_done = p_hide_on_done;
}

void AOMovie::play_file_name(QString p_file_name)
{
  m_movie->stop();
  m_movie->setFileName(p_file_name);
  m_first_loop = true;
  qDebug() << "playing" << p_file_name;
  m_movie->start();
  this->show();
}

void AOMovie::play(QString p_file, QString p_char)
{
  QString file_path = "";

  QString char_p_file;
  // FIXME: When looking in the character folder, append "_bubble" except for
  // custom We probably should drop this
  if (p_file != "custom")
    char_p_file = p_file + "_bubble";
  else
    char_p_file = p_file;

  // Asset lookup order
  // 1. If p_char is not empty, in the character folder, look for
  // `char_p_file` + extensions in `exts` in order
  // 2. If p_char is not empty, in the character folder, look for
  // `overlay/char_p_file` + extensions in `exts` in order
  // 3. In the theme folder (gamemode-timeofday/main/default), look for
  // `p_file` + extensions in `exts` in order
  // 4. In the theme folder (gamemode-timeofday/main/default), look for
  // "placeholder" + extensions in `exts` in order

  if (!p_char.isEmpty())
  {
    file_path = ao_app->find_asset_path(
        {
            ao_app->get_character_path(p_char, char_p_file),
            ao_app->get_character_path(p_char, "overlay/" + char_p_file),
        },
        animated_or_static_extensions());
  }

  if (file_path.isEmpty())
  {
    file_path = ao_app->find_theme_asset_path(p_file, animated_or_static_extensions());
    if (file_path.isEmpty())
      file_path = ao_app->find_theme_asset_path("placeholder", animated_or_static_extensions());
  }

  play_file_name(file_path);
}

///
/// \brief Searches and play the first interjection file it can find based on
/// the provided character name and interjection name.
///
void AOMovie::play_interjection(QString p_char_name, QString p_interjection_name)
{
  QString p_char_interjection_name;
  // FIXME: When looking in the character folder, append "_bubble" except for
  // custom We probably should drop this
  if (p_interjection_name.toLower() != "custom")
    p_char_interjection_name = p_interjection_name + "_bubble";
  else
    p_char_interjection_name = p_interjection_name;

  // Asset lookup order
  // 1. In the character folder, look for
  // `p_char_interjection_name`
  // 2. In the theme folder (gamemode-timeofday/main/default), look for
  // `p_char_name`

  QString interjection_filepath = ao_app->find_asset_path(
      {ao_app->get_character_path(p_char_name, p_char_interjection_name)}, animated_extensions());
  if (interjection_filepath.isEmpty())
    interjection_filepath = ao_app->find_theme_asset_path(p_interjection_name, animated_extensions());

  if (interjection_filepath.isEmpty())
  {
    Q_EMIT done();
    return;
  }

  qDebug() << "playing interjection" << (p_interjection_name.isEmpty() ? "(none)" : p_interjection_name)
           << "for character" << (p_char_name.isEmpty() ? "(none)" : p_char_name) << "at"
           << (interjection_filepath.isEmpty() ? "(not found)" : interjection_filepath);
  play_file_name(interjection_filepath);
}

void AOMovie::restart()
{
  m_movie->stop();
  m_movie->start();
}

void AOMovie::stop()
{
  m_movie->stop();
  // free up resources
  m_movie->setFileName(nullptr);
  if (m_hide_on_done)
    this->hide();
}

void AOMovie::frame_change(int n_frame)
{
  const QImage l_frame = m_movie->currentImage();
  bool l_paint_frame = true;

  if (m_play_once && n_frame == 0 && !m_first_loop)
  {
    l_paint_frame = false;
    stop();
  }

  if (l_paint_frame || m_first_loop)
  {
    m_first_loop = false;
    // paint
    AOPixmap l_pixmap(QPixmap::fromImage(l_frame));
    this->setPixmap(l_pixmap.scale(this->size()));
  }
}

void AOMovie::handle_state(QMovie::MovieState p_state)
{
  if (m_play_once && p_state == QMovie::NotRunning)
  {
    stop();
    Q_EMIT done();
  }
}

void AOMovie::combo_resize(int w, int h)
{
  QSize f_size(w, h);
  this->resize(f_size);
  m_movie->setScaledSize(f_size);
}

/*
 * @brief Returns the state of the current movie. Refer to QMovie::state()
 * for more details.
 * @returns Current movie status
 */
QMovie::MovieState AOMovie::state()
{
  return m_movie->state();
}
