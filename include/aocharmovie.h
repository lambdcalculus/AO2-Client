#ifndef AOCHARMOVIE_H
#define AOCHARMOVIE_H

#include "aopixmap.h"

#include <QLabel>
#include <QMovie>
#include <QTimer>

class AOApplication;

class AOCharMovie : public QLabel
{
  Q_OBJECT

public:
  AOCharMovie(QWidget *p_parent, AOApplication *p_ao_app);

  void play(QString p_chr, QString p_emote, QString emote_prefix, bool p_play_once);
  void play(QString p_chr, QString p_emote, bool p_play_once);
  bool play_pre(QString p_chr, QString p_emote);
  void play_talking(QString p_chr, QString p_emote);
  void play_idle(QString p_chr, QString p_emote);
  void set_mirror_enabled(bool p_enabled);
  void combo_resize(QSize p_size);
  void stop();

signals:
  void done();

private:
  AOApplication *ao_app = nullptr;

  QMovie *m_movie = nullptr;
  QTimer *m_frame_timer = nullptr;
  QImage m_current_frame;

  bool m_mirror = false;
  bool m_play_once = false;

  void paint_frame();

private slots:
  void on_frame_changed(int n_frame);
  void on_timer_timeout();
};

#endif // AOCHARMOVIE_H
