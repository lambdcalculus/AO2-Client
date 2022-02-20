#pragma once

#include "drstickermovie.h"

class DREffectMovie : public DRStickerMovie
{
  Q_OBJECT

public:
  explicit DREffectMovie(QWidget *parent = nullptr);
  ~DREffectMovie();
};
