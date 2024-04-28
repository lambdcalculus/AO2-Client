#include "localization_manager.h"
#include "aoapplication.h"

#include <QDir>

LocalizationManager LocalizationManager::s_Instance;

void LocalizationManager::execLoadLanguages()
{
  mLanguageNames.clear();
  mLanguageNames.append("English");
  QString l_localizationPath = AOApplication::getInstance()->get_base_path() + "localization/" ;
  QDir l_localizationDirectory(l_localizationPath);

  QStringList l_fileList = l_localizationDirectory.entryList(QStringList() << "*.json", QDir::Files);

  for(QString r_fileName : l_fileList)
  {
    QFileInfo l_fileInfo(l_localizationDirectory, r_fileName);

    LocalizationReader *l_newLocalization = new LocalizationReader(l_fileInfo.baseName());
    QString l_languageName = l_newLocalization->getLanguageName();
    if(!l_languageName.isEmpty())
    {
      mLanguages[l_languageName] = l_newLocalization;
      if(!mLanguageNames.contains(l_languageName)) mLanguageNames.append(l_languageName);
    };
  }

}

void LocalizationManager::setLanguage(QString t_languageName)
{
  mSelectedLanguageName = t_languageName;
}

QStringList LocalizationManager::getLanguageNames()
{
  return mLanguageNames;
}

QString LocalizationManager::getLocalizationText(QString t_value)
{
  if(mLanguages.contains(mSelectedLanguageName))
  {
    if(mLanguages[mSelectedLanguageName]->containsLocalizationValue(t_value)) return mLanguages[mSelectedLanguageName]->getLocalizationValue(t_value);
  }
  if(mDefaultText.contains(t_value))
  {
    return mDefaultText[t_value];
  }
  return "";
}

QString LocalizationManager::getLocalizationText(QString t_value, QStringList t_variables)
{
  if(mLanguages.contains(mSelectedLanguageName))
  {
    if(mLanguages[mSelectedLanguageName]->containsLocalizationValue(t_value)) return insertVariables(mLanguages[mSelectedLanguageName]->getLocalizationValue(t_value), t_variables);
  }
  if(mDefaultText.contains(t_value))
  {
    return insertVariables(mDefaultText[t_value], t_variables);
  }

  return "";
}

QString LocalizationManager::getLocalizationCode()
{
  if(mLanguages.contains(mSelectedLanguageName))
  {
    return mLanguages[mSelectedLanguageName]->getCode();
  }
  return "en";
}

QString LocalizationManager::getLocalizationCredit()
{
  if(mLanguages.contains(mSelectedLanguageName))
  {
    return mLanguages[mSelectedLanguageName]->getCredit();
  }
  return "";
}

QString LocalizationManager::insertVariables(QString t_value, QStringList t_variables)
{
  QString l_replacedText = t_value;

  for (int i = 0; i < t_variables.size(); ++i) {
    QString l_placeholder = "%" + QString::number(i);
    l_replacedText.replace(l_placeholder, t_variables[i]);
  }

  return l_replacedText;
}
