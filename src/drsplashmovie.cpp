#include "drsplashmovie.h"

DRSplashMovie::DRSplashMovie(QWidget *parent) : DRStickerMovie(parent)
{
  set_play_once(true);
  set_hide_on_done(true);
}

DRSplashMovie::~DRSplashMovie()
{}
