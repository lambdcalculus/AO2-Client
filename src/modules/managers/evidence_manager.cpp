#include "evidence_manager.h"
#include "drtextedit.h"

#include <modules/theme/thememanager.h>
#include "file_functions.h"


EvidenceManager EvidenceManager::s_Instance;


int EvidenceManager::getEvidenceCount()
{
  return mEvidenceList.count();
}

QString EvidenceManager::getEvidenceName(int t_id)
{
  if(mEvidenceList.count() >= (t_id + 1)) return mEvidenceList[t_id].mName;
  return "";
}

void EvidenceManager::updateSelectedEvidence(int t_id)
{
  if(mEvidenceList.count() >= (t_id + 1))
  {
    QString mText = mEvidenceList[t_id].mName + "\n\n" + mEvidenceList[t_id].mDescription;
    setEvidenceText(mText);
  }

  AOImageDisplay* l_evidenceImage = ThemeManager::get().getWidgetType<AOImageDisplay>("evidence_image");
  if(l_evidenceImage == nullptr) return;

  QString l_imagePath = AOApplication::getInstance()->get_base_file_path("evidence/" + mEvidenceList[t_id].mImage);
  if(file_exists(l_imagePath)) l_evidenceImage->set_image(l_imagePath);
  else
  {
    l_evidenceImage->set_theme_image("evidence_missing.png");
  }


}

void EvidenceManager::createDebugEvidence()
{
  EvidenceData dbgMonokumaFile;
  dbgMonokumaFile.mImage = "DR2/file_1.png";
  dbgMonokumaFile.mName = "Monokuma File #1";
  dbgMonokumaFile.mDescription = "The victim's body was discovered in the dining hall of the old building near the Hotel Mirai. The estimated time of death is 11:30 p.m. The victim was repeatedly stabbed over ten times between the throat and abdominal region, resulting in death. Aside from that, the body has no other external injuries, and no chemicals such as poison were detected.";
  mEvidenceList.append(dbgMonokumaFile);

  EvidenceData dbgKnife;
  dbgKnife.mName = "Knife";
  dbgKnife.mImage = "DR2/knife.png";
  dbgKnife.mDescription = "Bloodstained knife found under the table where the body was discovered. A glowing paint-like substance coats it.";
  mEvidenceList.append(dbgKnife);

  EvidenceData dbgBloodstain;
  dbgBloodstain.mName = "Bloodstain Under the Table";
  dbgBloodstain.mDescription = "A pool of blood formed around the body underneath the table. A lot of blood was splattered against the inside of the tablecloth. Also, there was no sign that the body was dragged through the bloodstain.";
  mEvidenceList.append(dbgBloodstain);

  EvidenceData dbgGaps;
  dbgGaps.mName = "Gaps in Floorboard";
  dbgGaps.mDescription = "The floors of the dining hall were full of gaps. Gundham dropped one of his earrings through these gaps.";
  mEvidenceList.append(dbgGaps);

  EvidenceData dbgTimer;
  dbgTimer.mImage = "DR2/timer.png";
  dbgTimer.mName = "A/C Timer";
  dbgTimer.mDescription = "The A/C timer in the dining hall was set to 11:30 p.m.";
  mEvidenceList.append(dbgTimer);

  EvidenceData dbgPose;
  dbgPose.mImage = "DR2/izuru_kamukura.png";
  dbgPose.mName = "Embarrassing Pose";
  dbgPose.mDescription = "During the blackout, Mikan tripped and fell in the dining hall. By the time the lights came back on, she ended up in a compromising position.";
  mEvidenceList.append(dbgPose);

  EvidenceData dbgCase;
  dbgCase.mName = "Duralumin Case";
  dbgCase.mImage = "DR2/case.png";
  dbgCase.mDescription = "The case Byakuya had with him. There's a lot of security equipment inside and a hard plastic case as well. The key to the other duralumin case is inside here too.";
  mEvidenceList.append(dbgCase);

}

void EvidenceManager::clearEvidence()
{
  mEvidenceList.clear();
}

void EvidenceManager::addEvidence(QString t_name, QString t_desc, QString t_image)
{
  EvidenceData l_newEvidence;
  l_newEvidence.mName = t_name;
  l_newEvidence.mDescription = t_desc;
  l_newEvidence.mImage = t_image;
  mEvidenceList.append(l_newEvidence);
}


void EvidenceManager::removeEvidence()
{

}

void EvidenceManager::setEvidenceText(QString t_text)
{
  DRTextEdit* l_evidenceText = ThemeManager::get().getWidgetType<DRTextEdit>("evidence_description");
  if(l_evidenceText != nullptr)
  {
    l_evidenceText->setText(t_text);
  }
}

int EvidenceManager::getCurrentPage()
{
  return mCurrentPage;
}

void EvidenceManager::setCurrentPage(int t_type)
{
  mCurrentPage = t_type;
  if(mCurrentPage < 0) mCurrentPage = 0;
}

void EvidenceManager::addCurrentPage(int t_type)
{
  mCurrentPage += t_type;
  if(mCurrentPage < 0) mCurrentPage = 0;
}
