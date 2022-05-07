#pragma once

#include <QLabel>
#include <QMovie>
#include <QTimer>

class DRMovie : public QLabel
{
  Q_OBJECT

public:
  enum ScalingMode
  {
    StretchScaling,
    WidthScaling,
    HeightScaling,
    DynamicScaling,
  };
  Q_ENUM(ScalingMode)

  explicit DRMovie(QWidget *parent = nullptr);
  ~DRMovie();

  QString file_name();
  void set_file_name(QString file_name);

  void set_play_once(bool);
  void set_mirrored(bool);
  void set_scale_mode(ScalingMode);
  void set_hide_on_done(bool);

  bool is_running();
  bool is_valid();
  void start();
  void stop();

signals:
  void done();

protected:
  virtual void resizeEvent(QResizeEvent *event) override;

private:
  bool m_play_once = false;
  bool m_mirrored = false;
  ScalingMode m_scaling_mode = StretchScaling;
  bool m_hide_when_done = false;

  QString m_file_name;
  bool m_file_exists = false;
  QMovie m_movie;
  int m_frame_count = 0;
  int m_frame_number = 0;
  QTimer m_frame_timer;
  QPixmap m_current_pixmap;

private slots:
  void update_frame(int);
  void jump_next_frame();
  void paint_frame();
};
