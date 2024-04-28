#ifndef EVIDENCELIST_H
#define EVIDENCELIST_H

#include "aobutton.h"
#include <QObject>
#include <QWidget>
#include "evidence_entry_button.h"

class EvidenceList : public QWidget
{
  Q_OBJECT
public:
  explicit EvidenceList(QWidget *parent = nullptr);
  bool buildList(int t_page);

private slots:
  void openAddEvidence();
  void displayContextMenu(QPoint t_position);

private:
  QVector<EvidenceEntryButton*> mEvidenceButtons = {};
  int mCurrentPage = 0;

signals:

};

#endif // EVIDENCELIST_H
