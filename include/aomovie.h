#ifndef AOMOVIE_H
#define AOMOVIE_H

#include <QLabel>
#include <QMovie>

class Courtroom;
class AOApplication;

class AOMovie : public QLabel
{
  Q_OBJECT

public:
  AOMovie(QWidget *p_parent, AOApplication *p_ao_app);
  ~AOMovie();

  void set_play_once(bool p_play_once);
  void play(QString p_file, QString p_char = "");

  ///
  /// \brief Searches and play the first interjection file it can find based on the provided character name and interjection name.
  ///
  void play_interjection(QString p_char_name, QString p_interjection_name);
  void combo_resize(int w, int h);
  void stop();

signals:
  void done();

private:
  QMovie *m_movie = nullptr;
  AOApplication *ao_app = nullptr;
  bool play_once = true;

private slots:
  void frame_change(int n_frame);
};

#endif // AOMOVIE_H
