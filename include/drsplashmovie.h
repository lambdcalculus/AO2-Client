#pragma once

#include "drstickermovie.h"

class DRSplashMovie : public DRStickerMovie
{
  Q_OBJECT

public:
  explicit DRSplashMovie(QWidget *parent = nullptr);
  ~DRSplashMovie();
};
