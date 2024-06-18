#include "evidence_list.h"
#include "aoapplication.h"
#include "courtroom.h"
#include "modules/theme/thememanager.h"
#include "modules/managers/evidence_manager.h"
#include "drevidencedialogue.h"

#include <qmenu.h>
EvidenceList::EvidenceList(QWidget *parent)
    : QWidget{parent}
{
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this ,&QWidget::customContextMenuRequested, this, &EvidenceList::displayContextMenu);
}

bool EvidenceList::buildList(int t_page)
{
  mCurrentPage = 0;
  while (!mEvidenceButtons.isEmpty())
    delete mEvidenceButtons.takeLast();
  mEvidenceButtons = {};

  int l_buttonHeight = 30;
  int l_verticalSpacing = 3;
  int l_listHeight = this->height();
  int l_maxEntries = l_listHeight / (l_buttonHeight + l_verticalSpacing);

  int l_startIndex = l_maxEntries * t_page;

  int l_evidenceCount = EvidenceManager::get().GetEvidenceCount();

  int endLoopIndex = l_startIndex + l_maxEntries;
  if (endLoopIndex > l_evidenceCount) {
    endLoopIndex = l_evidenceCount;
  }

  for (int i = l_startIndex; i < endLoopIndex; i++)
  {
    int r_relativePosition = i - l_startIndex;
    EvidenceEntryButton *l_newButton = new EvidenceEntryButton(this, AOApplication::getInstance());
    ThemeManager::get().AdjustWidgetTransform(l_newButton, 0, r_relativePosition * (l_buttonHeight + l_verticalSpacing));
    ThemeManager::get().AdjustWidgetDimensions(l_newButton, 225, l_buttonHeight);
    l_newButton->set_image("evidence_item.png");
    l_newButton->setEvidenceId(i);
    l_newButton->show();
    mEvidenceButtons.append(l_newButton);
  }

  if(l_startIndex + l_maxEntries < l_evidenceCount) return true;
  return false;
}

void EvidenceList::openAddEvidence()
{
  DREvidenceDialogue l_editor(this);
  l_editor.setWindowTitle(tr("Evidence Editor"));

  if (l_editor.exec())
  {
    EvidenceData l_evidence = l_editor.getEvidenceData();

    QStringList l_packet_contents;
    l_packet_contents.append(l_evidence.mName);
    l_packet_contents.append(l_evidence.mDescription);
    l_packet_contents.append(l_evidence.mImage);

    AOApplication::getInstance()->send_server_packet(DRPacket("EVA", l_packet_contents));
  }
}

void EvidenceList::displayContextMenu(QPoint t_position)
{
  QMenu *l_menu = new QMenu(this);
  QAction *a_addEvidenceMenu = new QAction("Add Evidence");
  QObject::connect(a_addEvidenceMenu, &QAction::triggered, [this](){openAddEvidence();});
  l_menu->addAction(a_addEvidenceMenu);
  l_menu->popup(this->mapToGlobal(t_position));
}
