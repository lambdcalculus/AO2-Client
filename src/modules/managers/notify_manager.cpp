#include "notify_manager.h"

NotifyManager NotifyManager::s_Instance;



void NotifyManager::ThemeSetupPopup(RPNotifyMenu *notify)
{
  if(pNotificationPopup != nullptr) delete pNotificationPopup;
  pNotificationPopup = notify;
  HideNotification();
}

void NotifyManager::SetPairNotifcation()
{
  mCurrentNotification = PairRequest;
  mCurrentNotificationMessage = mSenderName + " has sent you a pair request.";
  pNotificationPopup->SetNotificationText(mCurrentNotificationMessage);
  ShowNotification();
}

void NotifyManager::ShowNotification()
{
  pNotificationPopup->SetNotificationType(mCurrentNotification);
  pNotificationPopup->SetNotificationKey(mRequestKey);
  pNotificationPopup->SetNotificationSender(mSenderId);
  pNotificationPopup->show();
}

void NotifyManager::HideNotification()
{
  pNotificationPopup->hide();
}

void NotifyManager::ReloadNotification()
{
  if(pNotificationPopup != nullptr)
  {
    pNotificationPopup->ThemeReload();
  }
}

void NotifyManager::SetSenderId(int id)
{
  mSenderId = id;
}

void NotifyManager::SetSenderName(QString sender)
{
  mSenderName = sender;
}

void NotifyManager::SetSenderCharacter(QString sender)
{
  mSenderCharacter = sender;
}

void NotifyManager::SetRequestKey(QString sender)
{
  mRequestKey = sender;
}
