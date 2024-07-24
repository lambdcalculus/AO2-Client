#include "variable_manager.h"

#include <modules/theme/thememanager.h>

VariableManager VariableManager::s_Instance;

void VariableManager::setWatchlist(QMap<QString, QStringList > t_watchlist)
{
  mVariableWatchlist = t_watchlist;
}

void VariableManager::addWatchlist(QString t_freeblock, QString t_variable)
{
  if(!mVariableWatchlist[t_variable].contains(t_freeblock)) mVariableWatchlist[t_variable].append(t_freeblock);
}

void VariableManager::setVariable(QString t_name, QString t_variable)
{
  QString l_PreviousVariableName = "previous_" + t_name;

  if(mVariables.contains(t_name))
  {
    mVariables[l_PreviousVariableName] = mVariables[t_name];
    updateRequiredImages(l_PreviousVariableName);
  }

  mVariables[t_name] = t_variable;
  updateRequiredImages(t_name);
}

void VariableManager::updateRequiredImages(QString t_name)
{
  if(mVariableWatchlist.contains(t_name))
  {
    for(QString l_freeblock : mVariableWatchlist[t_name])
    {
      DRStickerViewer * lFreeblockSticker = ThemeManager::get().GetWidgetType<DRStickerViewer>(l_freeblock);
      if(lFreeblockSticker != nullptr)
      {
        lFreeblockSticker->setFreeblockImage(parseVariableString(lFreeblockSticker->getVariableString(), lFreeblockSticker->getVariableMap()), l_freeblock);
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

QString VariableManager::parseVariableString(VariableMappedString t_varMappedString)
{
  QString lOutput = t_varMappedString.mInputString;
  for(QString lKey : t_varMappedString.mVariableMap.keys())
  {
    lOutput.replace(lKey, getVariable(t_varMappedString.mVariableMap[lKey]));
  }
  return lOutput;
}

QString VariableManager::getVariable(QString t_name)
{
  if(mVariables.contains(t_name)) return mVariables[t_name];
  return "";
}
