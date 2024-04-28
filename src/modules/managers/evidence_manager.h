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

  int getEvidenceCount();
  QString getEvidenceName(int t_id);
  void updateSelectedEvidence(int t_id);

  void createDebugEvidence();
  void clearEvidence();
  void addEvidence(QString t_name, QString t_desc, QString t_image);
  void removeEvidence();
  void setEvidenceText(QString t_text);

  int getCurrentPage();
  void setCurrentPage(int t_type);
  void addCurrentPage(int t_type);

private:
  EvidenceManager() {}
  static EvidenceManager s_Instance;
  int mCurrentPage = 0;
  QVector<EvidenceData> mEvidenceList = {};
};

#endif // EVIDENCEMANAGER_H
