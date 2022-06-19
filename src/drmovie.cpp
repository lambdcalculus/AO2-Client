#include "drmovie.h"

#include <QDebug>
#include <QFile>

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
  m_file_exists = QFile::exists(m_file_name);
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
void DRMovie::set_scale_mode(DRMovie::ScalingMode p_mode)
{
  m_scaling_mode = p_mode;
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
  if (is_valid())
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
  QPixmap l_image = m_current_pixmap;

  {
    const QSize l_image_size = m_current_pixmap.size();
    const QSize l_widget_size = size();

    if (l_image_size != l_widget_size)
    {
      ScalingMode l_target_scaling_mode = m_scaling_mode;
      if (l_target_scaling_mode == DynamicScaling)
      {
        const qreal l_width_factor = (qreal)qMax(l_image_size.width(), 1) / qMax(l_widget_size.width(), 1);
        const qreal l_height_factor = (qreal)qMax(l_image_size.height(), 1) / qMax(l_widget_size.height(), 1);

        const QSize l_by_width_size{
            int((qreal)l_image_size.width() / l_width_factor),
            int((qreal)l_image_size.height() / l_width_factor),
        };
        const QSize l_by_height_size{
            int((qreal)l_image_size.width() / l_height_factor),
            int((qreal)l_image_size.height() / l_height_factor),
        };

        if (l_by_width_size.height() >= l_widget_size.height())
        {
          l_target_scaling_mode = WidthScaling;
        }
        else if (l_by_height_size.width() >= l_widget_size.width())
        {
          l_target_scaling_mode = HeightScaling;
        }
        else
        {
          l_target_scaling_mode = StretchScaling;
        }
      }

      Qt::TransformationMode l_transformation = Qt::SmoothTransformation;
      if (l_image_size.width() < l_widget_size.width() || l_image_size.height() < l_widget_size.height())
      {
        l_transformation = Qt::FastTransformation;
      }
      switch (l_target_scaling_mode)
      {
        case StretchScaling:
          [[fallthrough]];
        default:
          l_image = l_image.scaled(l_widget_size, Qt::IgnoreAspectRatio, l_transformation);
          break;

        case WidthScaling:
          l_image = l_image.scaledToWidth(l_widget_size.width(), l_transformation);
          break;

        case HeightScaling:
          l_image = l_image.scaledToHeight(l_widget_size.height(), l_transformation);
          break;
      }
    }
  }

  setPixmap(l_image);
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

bool DRMovie::is_valid()
{
  return m_file_exists && m_movie.isValid();
}
