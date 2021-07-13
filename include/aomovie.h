#ifndef AOMOVIE_H
#define AOMOVIE_H

class AOApplication;

#include <QLabel>
#include <QMovie>

class AOMovie : public QLabel
{
  Q_OBJECT

public:
  AOMovie(QWidget *p_parent, AOApplication *p_ao_app);
  ~AOMovie();

  bool is_play_once();
  void set_play_once(bool p_play_once);

  bool is_hide_on_done();
  void set_hide_on_done(bool p_hide_on_done);

  void play_file_name(QString file);
  void play(QString p_file, QString p_char = "");
  void play_interjection(QString p_char_name, QString p_interjection_name);
  void restart();
  void combo_resize(int w, int h);
  QMovie::MovieState state();
  void stop();

signals:
  void done();

private:
  AOApplication *ao_app = nullptr;

  QMovie *m_movie = nullptr;
  bool m_first_loop = false;
  bool m_play_once = true;
  bool m_hide_on_done = false;

private slots:
  void frame_change(int n_frame);
  void handle_state(QMovie::MovieState);
};

#endif // AOMOVIE_H
