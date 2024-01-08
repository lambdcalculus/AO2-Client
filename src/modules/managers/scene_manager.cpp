#include "scene_manager.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

SceneManager SceneManager::s_Instance;

AOLabel *SceneManager::CreateTransition(QWidget *parents, AOApplication *ao_app, DRGraphicsView *viewport)
{
  pUiTransition = new AOLabel(parents, ao_app);
  pViewport = viewport;
  return pUiTransition;
}

AOLabel *SceneManager::GetTransition()
{
  return pUiTransition;
}


void SceneManager::RenderTransition()
{
  QImage image(pViewport->scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  pViewport->scene()->render(&painter);
  pUiTransition->setPixmap(QPixmap::fromImage(image));
}




void SceneManager::AnimateTransition()
{
  QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
  pUiTransition->setGraphicsEffect(eff);
  QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
  a->setDuration(mFadeDuration);
  a->setStartValue(1);
  a->setEndValue(0);
  a->setEasingCurve(QEasingCurve::OutBack);
  a->start(QPropertyAnimation::DeleteWhenStopped);
}

void SceneManager::setFadeDuration(int duration)
{
  mFadeDuration = duration;
}

