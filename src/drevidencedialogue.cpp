#include "drevidencedialogue.h"
#include "ui_drevidencedialogue.h"
#include "aoapplication.h"
#include <QDir>

DREvidenceDialogue::DREvidenceDialogue(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::DREvidenceDialogue)
{
  ui->setupUi(this);

  ui->wSeriesCombo->addItem("default");
  QDir l_evidenceDirectory(AOApplication::getInstance()->get_base_path() + "evidence");

  QStringList l_evidenceSubdirectories = l_evidenceDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  if(l_evidenceSubdirectories.length() <= 0) return;
  for (const QString &f_evidencePath : l_evidenceSubdirectories)
  {
    QDir l_outfitPathFull(l_evidenceDirectory.filePath(f_evidencePath));
    if(f_evidencePath != "default") ui->wSeriesCombo->addItem(f_evidencePath);
  }
}

DREvidenceDialogue::~DREvidenceDialogue()
{
  delete ui;
}

EvidenceData DREvidenceDialogue::getEvidenceData()
{
  EvidenceData l_return = EvidenceData();
  l_return.mName = ui->wName->text();
  l_return.mImage = ui->wSeriesCombo->currentText() + "/" + ui->wIconName->currentText();
  l_return.mDescription = ui->wDescription->toPlainText();

  return l_return;
}

void DREvidenceDialogue::on_wSeriesCombo_currentTextChanged(const QString &arg1)
{
  ui->wIconName->clear();
  QDir l_evidenceDirectory(AOApplication::getInstance()->get_base_path() + "evidence/" + arg1);

  QStringList l_evidenceSubdirectories = l_evidenceDirectory.entryList(QDir::Files);

  if(l_evidenceSubdirectories.length() <= 0) return;
  for (const QString &f_evidencePath : l_evidenceSubdirectories)
  {
    QDir l_outfitPathFull(l_evidenceDirectory.filePath(f_evidencePath));
    ui->wIconName->addItem(f_evidencePath);
  }
}


void DREvidenceDialogue::on_wIconName_currentTextChanged(const QString &arg1)
{
  ui->label_4->setPixmap(AOApplication::getInstance()->get_base_path() + "evidence/" + ui->wSeriesCombo->currentText() + "/" + ui->wIconName->currentText());
}

