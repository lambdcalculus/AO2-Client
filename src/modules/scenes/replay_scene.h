#ifndef REPLAYSCENE_H
#define REPLAYSCENE_H

#include <QObject>
#include <QWidget>
#include <drcharactermovie.h>
#include "drgraphicscene.h"
#include "drscenemovie.h"
#include <aoapplication.h>

class ReplayScene : public QWidget
{
  Q_OBJECT
public:
  ReplayScene(AOApplication *p_ao_app, QWidget *parent = nullptr);

private:
  AOApplication *pAOApp = nullptr;

  void constructWidgets();

  void setBackground(QString t_name);
  void setBgPosition(QString t_name);


private: //Viewport
  DRSceneMovie *vpBackground = nullptr;
  DRCharacterMovie *vpPlayer = nullptr;


};

#endif // REPLAYSCENE_H
