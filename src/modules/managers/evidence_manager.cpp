#include "evidence_manager.h"
#include "drtextedit.h"

#include <modules/theme/thememanager.h>
#include "file_functions.h"


EvidenceManager EvidenceManager::s_Instance;


int EvidenceManager::GetEvidenceCount()
{
  return m_EvidenceList.count();
}

QString EvidenceManager::GetEvidenceName(int t_id)
{
  if(m_EvidenceList.count() >= (t_id + 1)) return m_EvidenceList[t_id].mName;
  return "";
}

void EvidenceManager::SelectEvidence(int t_id)
{
  if(m_EvidenceList.count() >= (t_id + 1))
  {
    QString mText = m_EvidenceList[t_id].mName + "\n\n" + m_EvidenceList[t_id].mDescription;
    SetEvidenceDescription(mText);
  }

  AOImageDisplay* l_evidenceImage = ThemeManager::get().GetWidgetType<AOImageDisplay>("evidence_image");
  if(l_evidenceImage == nullptr) return;

  QString l_imagePath = AOApplication::getInstance()->get_base_file_path("evidence/" + m_EvidenceList[t_id].mImage);
  if(file_exists(l_imagePath)) l_evidenceImage->set_image(l_imagePath);
  else
  {
    l_evidenceImage->set_theme_image("evidence_missing.png");
  }


}

void EvidenceManager::DestroyAllEvidence()
{
  m_EvidenceList.clear();
}

void EvidenceManager::CreateEvidence(QString t_name, QString t_desc, QString t_image)
{
  EvidenceData l_newEvidence;
  l_newEvidence.mName = t_name;
  l_newEvidence.mDescription = t_desc;
  l_newEvidence.mImage = t_image;
  m_EvidenceList.append(l_newEvidence);
}


void EvidenceManager::DestroyEvidence()
{

}

void EvidenceManager::SetEvidenceDescription(QString t_text)
{
  DRTextEdit* l_evidenceText = ThemeManager::get().GetWidgetType<DRTextEdit>("evidence_description");
  if(l_evidenceText != nullptr)
  {
    l_evidenceText->setText(t_text);
  }
}

int EvidenceManager::GetCurrentPage()
{
  return m_PageCurrentID;
}

void EvidenceManager::SetCurrentPage(int t_type)
{
  m_PageCurrentID = t_type;
  if(m_PageCurrentID < 0) m_PageCurrentID = 0;
}

void EvidenceManager::AddCurrentPage(int t_type)
{
  m_PageCurrentID += t_type;
  if(m_PageCurrentID < 0) m_PageCurrentID = 0;
}
