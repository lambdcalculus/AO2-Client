#ifndef RPNOTIFYMENU_H
#define RPNOTIFYMENU_H

#include <QWidget>
#include <aobutton.h>
#include <drstickerviewer.h>
#include <drtextedit.h>

enum NotificationType
{
  INVALID = 0,
  PairRequest = 1,
  PartyRequest = 2,
  EventPopup = 3

};

class RPNotifyMenu : public QWidget
{
  Q_OBJECT
public:
  explicit RPNotifyMenu(QWidget *parent = nullptr);

  void SetNotificationText(QString message);

  void SetNotificationType(NotificationType notify);
  void SetNotificationKey(QString key);
  void SetNotificationSender(int sender);


private:

  DRStickerViewer *pBackgroundImage = nullptr;
  AOButton *pAcceptButton = nullptr;
  AOButton *pDeclineButton = nullptr;
  DRTextEdit *pMessageText = nullptr;

  void NotifyAccept();
  void NotifyDecline();

  NotificationType mType = INVALID;
  int mSender = -1;
  QString mKey = "";

signals:

};

#endif // RPNOTIFYMENU_H
