#ifndef EVIDENCEENTRYBUTTON_H
#define EVIDENCEENTRYBUTTON_H

#include <aobutton.h>
#include <QObject>
#include <QWidget>
#include <drtextedit.h>

class EvidenceEntryButton : public AOButton
{
  Q_OBJECT
public:
  EvidenceEntryButton(QWidget *parent, AOApplication *p_ao_app);
  void setEvidenceId(int t_id);

private slots:
  void on_clicked();

private:
  DRTextEdit *wEvidenceName = nullptr;
  int mEvidenceId = -1;
  bool mEvidenceToggled = false;
};

#endif // EVIDENCEENTRYBUTTON_H
