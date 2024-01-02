#include "thememanager.h"


ThemeManager ThemeManager::s_Instance;

void ThemeManager::setResize(double size)
{
  mClientResize = size;
}

double ThemeManager::getResize()
{
  return mClientResize;
}
