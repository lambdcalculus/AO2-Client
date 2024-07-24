#ifndef VARIABLEMANAGER_H
#define VARIABLEMANAGER_H

#include "drstickerviewer.h"
#include "datatypes.h"
#include <QString>
#include <qmap.h>



class VariableManager
{
public:
  VariableManager(const VariableManager&) = delete;

  static VariableManager& get()
  {
    return s_Instance;
  }

  void setWatchlist(QMap<QString, QStringList> t_watchlist);
  void addWatchlist(QString t_freeblock, QString t_variable);
  void setVariable(QString t_name, QString t_variable);

  void updateRequiredImages(QString t_name);

  QString parseVariableString(QString t_name, QMap<QString, QString> t_map);
  QString parseVariableString(VariableMappedString t_varMappedString);
  QString getVariable(QString t_name);

private:

  QMap<QString, QString> mVariables = {};
  VariableManager() {}
  static VariableManager s_Instance;

  QMap<QString, QStringList> mVariableWatchlist = {};
};

#endif // VARIABLEMANAGER_H
