#include "drmovie.h"

#include <QDebug>

DRMovie::DRMovie(QWidget *parent) : QLabel(parent)
{
  setAlignment(Qt::AlignCenter);

  m_movie.setCacheMode(QMovie::CacheNone);
  m_frame_timer.setSingleShot(true);
  m_frame_timer.setTimerType(Qt::PreciseTimer);

  connect(&m_movie, SIGNAL(frameChanged(int)), this, SLOT(update_frame(int)));
  connect(&m_frame_timer, SIGNAL(timeout()), this, SLOT(jump_next_frame()));
}

DRMovie::~DRMovie()
{}

QString DRMovie::file_name()
{
  return m_file_name;
}

/**
 * @brief Sets the property filename.
 *
 * stop() will be called prior to updating setting the property.
 */
void DRMovie::set_file_name(QString p_file_name)
{
  stop();
  m_file_name = p_file_name;
}

/**
 * @brief Sets the property play_once on if true or off if false.
 *
 * If on, the movie will repeat until either conditions are met:
 * - setting play_once to off
 * - calling stop()
 *
 * The signal done() will not be emitted while play_once is on.
 *
 * By default, play_once is off.
 */
void DRMovie::set_play_once(bool p_on)
{
  m_play_once = p_on;
}

/**
 * @brief Sets the property hide_when_done on if on true or off if false.
 *
 * If on, the widget will hide automatically right before signaling done()
 *
 * By default, hide_when_done is off.
 */
void DRMovie::set_hide_on_done(bool p_on)
{
  m_hide_when_done = p_on;
}

/**
 * @brief Sets the property mirrored on if true or off if false.
 *
 * If on, painted frames will be mirrored horizontally.
 *
 * The current frame will NOT be updated.
 *
 * By default, mirrored is off.
 */
void DRMovie::set_mirrored(bool p_on)
{
  m_mirrored = p_on;
}

/**
 * @brief Sets the property scale_to_height on if true or off if false.
 *
 * If on, painted frames will preserve their aspect ratio and scale based
 * on the height of the widget.
 *
 * The current frame will NOT be updated.
 *
 * By default, scale_to_height is off.
 */
void DRMovie::set_scale_to_height(bool p_on)
{
  m_scale_to_height = p_on;
}

/**
 * @brief Starts (or restarts) the movie.
 *
 * If the movie isn't valid and the property play_once is on,
 * the movie will immediately emit the signal done()
 */
void DRMovie::start()
{
  m_frame_timer.stop();
  m_movie.stop();
  m_movie.setFileName(m_file_name);
  m_frame_count = m_movie.frameCount();
  m_frame_number = m_movie.currentFrameNumber();
  if (m_movie.isValid())
  {
    m_movie.start();
    m_movie.setPaused(true);
    show();
  }
  else if (m_play_once)
  {
    emit done();
  }
}

bool DRMovie::is_running()
{
  return m_movie.state() != QMovie::NotRunning;
}

void DRMovie::stop()
{
  m_frame_timer.stop();
  m_movie.stop();
  m_movie.setFileName(""); // clean up QMovie's cache

  if (m_hide_when_done)
  {
    hide();
  }
}

void DRMovie::resizeEvent(QResizeEvent *event)
{
  QLabel::resizeEvent(event);

  paint_frame();
}

void DRMovie::paint_frame()
{
  QPixmap l_frame = m_current_pixmap;

  const bool l_is_larger = l_frame.width() > width() || l_frame.height() > height();
  const Qt::TransformationMode l_transform = l_is_larger ? Qt::SmoothTransformation : Qt::FastTransformation;
  if (m_scale_to_height)
  {
    l_frame = l_frame.scaledToHeight(height(), l_transform);
  }
  else
  {
    l_frame = l_frame.scaled(size(), Qt::IgnoreAspectRatio, l_transform);
  }

  setPixmap(l_frame);
}

void DRMovie::update_frame(int p_frame_number)
{
  m_frame_number = p_frame_number;
  m_frame_timer.start(m_movie.nextFrameDelay());

  if (m_mirrored)
  {
    m_current_pixmap = QPixmap::fromImage(m_movie.currentImage().mirrored(m_mirrored, false));
  }
  else
  {
    m_current_pixmap = m_movie.currentPixmap();
  }

  paint_frame();
}

void DRMovie::jump_next_frame()
{
  const int l_next_frame_number = m_frame_number + 1;
  if (l_next_frame_number >= m_frame_count)
  {
    m_movie.stop();
    m_frame_number = m_movie.currentFrameNumber();

    if (m_play_once)
    {
      if (m_hide_when_done)
      {
        hide();
      }

      emit done();
      return;
    }

    if (m_frame_count > 1)
    {
      m_movie.start();
      m_movie.setPaused(true);
    }
  }
  else
  {
    m_movie.jumpToNextFrame();
  }
}
