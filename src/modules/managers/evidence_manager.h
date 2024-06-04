#ifndef EVIDENCEMANAGER_H
#define EVIDENCEMANAGER_H

#include "datatypes.h"

class EvidenceManager
{
public:
  EvidenceManager(const EvidenceManager&) = delete;

  static EvidenceManager& get()
  {
    return s_Instance;
  }

  //Evidence Data Management
  void DestroyEvidence();
  void DestroyAllEvidence();
  void CreateEvidence(QString t_name, QString t_desc, QString t_image);
  int GetEvidenceCount();
  QString GetEvidenceName(int t_id);

  //Widget Management
  void SelectEvidence(int t_id);
  void SetEvidenceDescription(QString t_text);

  //Page Management
  int GetCurrentPage();
  void SetCurrentPage(int t_type);
  void AddCurrentPage(int t_type);

private:
  EvidenceManager() {}
  static EvidenceManager s_Instance;

private:
  QVector<EvidenceData> m_EvidenceList = {};
  int m_PageCurrentID = 0;
};

#endif // EVIDENCEMANAGER_H
