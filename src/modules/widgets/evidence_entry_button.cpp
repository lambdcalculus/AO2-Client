#include "evidence_entry_button.h"
#include "AOApplication.h"
#include "commondefs.h"
#include "modules/managers/evidence_manager.h"
#include "theme.h"
EvidenceEntryButton::EvidenceEntryButton(QWidget *parent, AOApplication *p_ao_app) : AOButton(parent, p_ao_app)
{
  wEvidenceName = new DRTextEdit(this);
  wEvidenceName->setFrameStyle(QFrame::NoFrame);
  wEvidenceName->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  wEvidenceName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  wEvidenceName->setReadOnly(true);
  wEvidenceName->setAttribute(Qt::WA_TransparentForMouseEvents);

  set_drtextedit_font(wEvidenceName, "evidence_list", COURTROOM_FONTS_INI, AOApplication::getInstance());

  connect(this, &QAbstractButton::clicked, this, &EvidenceEntryButton::on_clicked);
}

void EvidenceEntryButton::setEvidenceId(int t_id)
{
  mEvidenceId = t_id;
  wEvidenceName->move(0, 0);
  wEvidenceName->resize(this->width(), this->height());
  wEvidenceName->setPlainText(EvidenceManager::get().getEvidenceName(mEvidenceId));
}

void EvidenceEntryButton::on_clicked()
{
  if(mEvidenceId < 0) return;
  EvidenceManager::get().updateSelectedEvidence(mEvidenceId);
}
