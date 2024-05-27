#include "typewritertextedit.h"
#include "qregularexpression.h"
#include "aoapplication.h"

#include <modules/managers/game_manager.h>

TypewriterTextEdit::TypewriterTextEdit(QWidget *p_parent) : DRTextEdit(p_parent)
{
  pBlipPlayer = new AOBlipPlayer(AOApplication::getInstance(), this);
  pBlipPlayer->set_blips("sfx-blipmale.wav");
}

void TypewriterTextEdit::setTypewriterTarget(QString t_text)
{
  setText("");
  mLastUpdate = mStartTime;
  mText = "";
  mRenderedText = "";
  mCurrentColor = ' ';
  mCurrentIndex = 0;


  if(t_text.trimmed().isEmpty()) return;
  bool l_ignoreNextLetter = false;
  for (QChar r_char : t_text)
  {

    const QChar f_character = r_char;

    if (!l_ignoreNextLetter && f_character == Qt::Key_Backslash)
    {
      l_ignoreNextLetter = true;
      continue;
    }
    else if (!l_ignoreNextLetter && (f_character == Qt::Key_BraceLeft || f_character == Qt::Key_BraceRight)) // { or }
    {
      if(f_character == Qt::Key_BraceRight)
      {
        mTypeWriterActions[mText.count()] = eTypeWriterSpeedUp;
      }
      else
      {
        mTypeWriterActions[mText.count()] = eTypeWriterSpeedDown;
      }
      continue;
    }
    else if (l_ignoreNextLetter && f_character == 's')
    {
      l_ignoreNextLetter = false;
      mTypeWriterActions[mText.count()] = eTypeWriterShake;
      continue;
    }
    else if (f_character == Qt::Key_Space)
    {
      mText.append(f_character);
    }
    else
    {
      mText.append(f_character);
    }
    l_ignoreNextLetter = false;
  }
  progressLetter();
  progressLetter();
  progressLetter();
}

void TypewriterTextEdit::progressLetter()
{
  //bool m_chatbox_message_enable_highlighting = (AOApplication::getInstance()->current_theme->read_config_bool("enable_highlighting"));

  if((GameManager::get().getUptime() - mLastUpdate) < mBlipRate)
  {
    return;
  };
  if(mCurrentIndex >= mText.count()) return;
  if(mCurrentIndex == 0) pBlipPlayer->blip_tick();

  QTextCharFormat l_currentCharFormat = currentCharFormat();

  QChar l_newChar = mText.at(mCurrentIndex);
  QVector<QStringList> m_chatbox_message_highlight_colors = AOApplication::getInstance()->get_highlight_colors();

  if(mCurrentColor == ' ')
  {
    QColor text_color;
    text_color.setNamedColor("#F9FFFE");
    l_currentCharFormat.setForeground(text_color);
  }
  for(QStringList mMessageHighlights : m_chatbox_message_highlight_colors)
  {
    if(mMessageHighlights[0].at(0) == mCurrentColor)
    {
      if(l_newChar == mMessageHighlights[0].at(1))
      {
        mCurrentColor = ' ';
      }
    }
    else if(mMessageHighlights[0].at(0) == l_newChar)
    {
      QColor text_color;
      text_color.setNamedColor(mMessageHighlights[1]);
      l_currentCharFormat.setForeground(text_color);
      mCurrentColor = l_newChar;
    }
  }

  textCursor().insertText(l_newChar, l_currentCharFormat);
  mRenderedText.append(l_newChar);

  //setText(mRenderedText);
  mCurrentIndex += 1;
  mLastUpdate = GameManager::get().getUptime();
}

bool TypewriterTextEdit::isTextRendered()
{
  return mRenderedText == mText;
}

