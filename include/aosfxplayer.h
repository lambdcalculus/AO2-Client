#pragma once

#include "aoobject.h"

class AOSfxPlayer : public AOObject
{
  Q_OBJECT

public:
  AOSfxPlayer(AOApplication *p_ao_app, QObject *p_parent = nullptr);

  void play(QString p_file);
};
