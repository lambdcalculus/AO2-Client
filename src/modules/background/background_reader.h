#ifndef BACKGROUNDREADER_H
#define BACKGROUNDREADER_H

#include "background_data.h"
#include <modules/json/json_reader.h>



class BackgroundReader : public BackgroundData, public JSONReader
{
public:
  BackgroundReader();

  void execLoadBackground(QString t_backgroundName);
  int getVerticalPosition(double t_height);
};

#endif // BACKGROUNDREADER_H
