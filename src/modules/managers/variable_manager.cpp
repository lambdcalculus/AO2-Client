#include "variable_manager.h"

#include <modules/theme/thememanager.h>

VariableManager VariableManager::s_Instance;

void VariableManager::setWatchlist(QMap<QString, QStringList > t_watchlist)
{
  mVariableWatchlist = t_watchlist;
}

void VariableManager::addWatchlist(QString t_freeblock, QString t_variable)
{
  //if(!mVariableWatchlist.contains(t_variable))
  //{
  //  mVariableWatchlist[t_variable];
  //}
  if(!mVariableWatchlist[t_variable].contains(t_freeblock)) mVariableWatchlist[t_variable].append(t_freeblock);
}

void VariableManager::setVariable(QString t_name, QString t_variable)
{
  mVariables[t_name] = t_variable;
  if(mVariableWatchlist.contains(t_name))
  {
    for(QString l_freeblock : mVariableWatchlist[t_name])
    {
      DRStickerViewer * lFreeblockSticker = ThemeManager::get().getWidgetType<DRStickerViewer>(l_freeblock);
      if(lFreeblockSticker != nullptr)
      {
        lFreeblockSticker->set_theme_image(parseVariableString(lFreeblockSticker->getVariableString(), lFreeblockSticker->getVariableMap()));
        lFreeblockSticker->start();
      }
    }
  }
}

QString VariableManager::parseVariableString(QString t_name, QMap<QString, QString> t_map)
{
  QString lOutput = t_name;
  for(QString lKey : t_map.keys())
  {
    lOutput.replace(lKey, getVariable(t_map[lKey]));
  }
  return lOutput;
}

QString VariableManager::getVariable(QString t_name)
{
  if(mVariables.contains(t_name)) return mVariables[t_name];
  return "";
}
