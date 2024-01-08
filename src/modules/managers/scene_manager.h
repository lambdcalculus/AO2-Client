#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "src/aolabel.h"
#include "src/drgraphicscene.h"

class SceneManager
{
public:
  SceneManager(const SceneManager&) = delete;

  static SceneManager& get()
  {
    return s_Instance;
  }

  AOLabel *CreateTransition(QWidget *parents, AOApplication* ao_app, DRGraphicsView *viewport);
  AOLabel *GetTransition();

  void RenderTransition();
  void AnimateTransition();
  void setFadeDuration(int duration);
private:
  SceneManager() {}
  static SceneManager s_Instance;

  int mFadeDuration = 200;
  AOLabel *pUiTransition = nullptr;
  DRGraphicsView *pViewport = nullptr;

};

#endif // SCENEMANAGER_H
