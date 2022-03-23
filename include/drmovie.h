#pragma once

#include <QLabel>
#include <QMovie>
#include <QTimer>

class DRMovie : public QLabel
{
  Q_OBJECT

public:
  explicit DRMovie(QWidget *parent = nullptr);
  ~DRMovie();

  QString file_name();
  void set_file_name(QString file_name);

  void set_play_once(bool);
  void set_mirrored(bool);
  void set_scale_to_height(bool);
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
  bool m_scale_to_height = false;
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
