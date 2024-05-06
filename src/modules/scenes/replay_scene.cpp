#include "replay_scene.h"
#include "modules/theme/thememanager.h"
#include "modules/managers/scene_manager.h"

ReplayScene::ReplayScene(AOApplication *p_ao_app, QWidget *parent) : QWidget(parent)
{
  pAOApp = p_ao_app;
  constructWidgets();
  setWindowTitle("Replay");
}

void ReplayScene::constructWidgets()
{
  ThemeManager::get().SetWidgetNames({});
  ThemeManager::get().createButtonWidget("return_to_lobby", this);

  DRGraphicsView *l_viewport = new DRGraphicsView(this);
  ThemeManager::get().autoWidgetDimensions(l_viewport, "viewport", REPLAYS);
  ThemeManager::get().addWidgetName("viewport", l_viewport);


  vpBackground = new DRSceneMovie(pAOApp);
  vpPlayer = new DRCharacterMovie(pAOApp);

  l_viewport->scene()->addItem(vpBackground);
  l_viewport->scene()->addItem(vpPlayer);

  vpBackground->start();

}

void ReplayScene::setBackground(QString t_name)
{
  SceneManager::get().execLoadPlayerBackground(t_name);
  setBgPosition("wit");
}

void ReplayScene::setBgPosition(QString t_name)
{
  vpBackground->set_file_name(SceneManager::get().getBackgroundPath(t_name));
}
