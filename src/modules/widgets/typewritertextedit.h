#ifndef TYPEWRITERTEXTEDIT_H
#define TYPEWRITERTEXTEDIT_H

#include <drtextedit.h>
#include <QObject>
#include <QWidget>
#include <aoblipplayer.h>

enum TypeWriterAction
{
  eTypeWriterText,
  eTypeWriterShake,
  eTypeWriterSpeedUp,
  eTypeWriterSpeedDown
};

class TypewriterTextEdit : public DRTextEdit
{
  Q_OBJECT
public:
  TypewriterTextEdit(QWidget *p_parent = nullptr);

  void setTypewriterTarget(QString t_text);
  void UpdateDisplay();
  bool GetTypingComplete();

private:

  int mCurrentIndex = 0;

  QString mText = "";
  QString mRenderedText = "";

  //Timings
  int m_TimingFirstUpdate = 0;
  int m_TimingLastUpdate = 0;

  //Blips
  AOBlipPlayer *p_BlipAudioPlayer = nullptr;
  int mBlipRate = 30;

  QChar mCurrentColor = ' ';
  QVector<QString> m_QueuedHighlights = {};
  QMap<int, TypeWriterAction> m_QueuedActions = {};
};

#endif // TYPEWRITERTEXTEDIT_H
