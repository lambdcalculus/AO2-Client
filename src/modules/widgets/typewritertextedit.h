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
  void progressLetter();
  bool isTextRendered();

private:
  QMap<int, TypeWriterAction> mTypeWriterActions = {};

  int mCurrentIndex = 0;

  QString mText = "";
  QString mRenderedText = "";

  //Timings
  int mStartTime = 0;
  int mBlipRate = 30;
  int mLastUpdate = 0;

  AOBlipPlayer *pBlipPlayer = nullptr;

  QChar mCurrentColor = ' ';
};

#endif // TYPEWRITERTEXTEDIT_H
