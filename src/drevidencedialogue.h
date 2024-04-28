#ifndef DREVIDENCEDIALOGUE_H
#define DREVIDENCEDIALOGUE_H

#include <QDialog>
#include "datatypes.h"

namespace Ui {
class DREvidenceDialogue;
}

class DREvidenceDialogue : public QDialog
{
  Q_OBJECT

public:
  explicit DREvidenceDialogue(QWidget *parent = nullptr);
  ~DREvidenceDialogue();
  EvidenceData getEvidenceData();

private slots:
  void on_wSeriesCombo_currentTextChanged(const QString &arg1);

  void on_wIconName_currentTextChanged(const QString &arg1);

private:
  Ui::DREvidenceDialogue *ui;
};

#endif // DREVIDENCEDIALOGUE_H
