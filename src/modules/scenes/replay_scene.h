#ifndef REPLAYSCENE_H
#define REPLAYSCENE_H

#include <QObject>
#include <QWidget>
#include <drcharactermovie.h>
#include "drgraphicscene.h"
#include "drscenemovie.h"
#include <aoapplication.h>
#include <drtextedit.h>
#include "modules/widgets/typewritertextedit.h"

class ReplayScene : public QWidget
{
  Q_OBJECT
public:
  ReplayScene(AOApplication *p_ao_app, QWidget *parent = nullptr);


  void setText(QString t_dialogue);
  void setCharacter(QString t_name, QString t_emote);
  void setBackground(QString t_name);
  void setBgPosition(QString t_name);

private:
  AOApplication *pAOApp = nullptr;

  void constructWidgets();



protected:
  void keyPressEvent(QKeyEvent *event) override;

private: //Viewport
  DRSceneMovie *vpBackground = nullptr;
  DRCharacterMovie *vpPlayer = nullptr;
  DRTextEdit *vpShowname = nullptr;
  TypewriterTextEdit *vpMessage = nullptr;


};

#endif // REPLAYSCENE_H
