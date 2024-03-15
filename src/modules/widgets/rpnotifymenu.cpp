#include "rpnotifymenu.h"
#include "AOApplication.h"
#include "commondefs.h"
#include "drtheme.h"
#include "file_functions.h"
#include "theme.h"

RPNotifyMenu::RPNotifyMenu(QWidget *parent)
    : QWidget{parent}
{
  AOApplication *ao_app = AOApplication::getInstance();

  pBackgroundImage = new DRStickerViewer(ao_app, this);
  pMessageText = new DRTextEdit(pBackgroundImage);
  pAcceptButton = new AOButton(pBackgroundImage, ao_app);
  pDeclineButton = new AOButton(pBackgroundImage, ao_app);

  pBackgroundImage->move(0, 0);
  pBackgroundImage->resize(235, 135);

  if (!ao_app->find_theme_asset_path("notify_bg", animated_or_static_extensions()).isEmpty())
  {
    pBackgroundImage->set_theme_image("notify_bg");
  }
  set_sticker_play_once(pBackgroundImage, "notify_bg", COURTROOM_CONFIG_INI, ao_app);


  pMessageText->setFrameStyle(QFrame::NoFrame);
  pMessageText->move(5, 5);
  pMessageText->resize(224, 95);

  pMessageText->setReadOnly(true);
  set_drtextedit_font(pMessageText, "notify_popup", COURTROOM_FONTS_INI, ao_app);
  pMessageText->setPlainText(pMessageText->toPlainText());

  pAcceptButton->move(10, 100);
  pAcceptButton->resize(100, 30);

  pAcceptButton->set_image(ao_app->current_theme->get_widget_image("notify_accept", "notify_accept.png", "courtroom"));
  if (pAcceptButton->get_image().isEmpty())
    pAcceptButton->setText("Accept");


  pDeclineButton->move(125, 100);
  pDeclineButton->resize(100, 30);

  pDeclineButton->set_image(ao_app->current_theme->get_widget_image("notify_decline", "notify_decline.png", "courtroom"));
  if (pDeclineButton->get_image().isEmpty())
    pDeclineButton->setText("Decline");


  connect(pAcceptButton, &QAbstractButton::clicked, this, &RPNotifyMenu::NotifyAccept);
  connect(pDeclineButton, &QAbstractButton::clicked, this, &RPNotifyMenu::NotifyDecline);


}

void RPNotifyMenu::SetNotificationText(QString message)
{
  pMessageText->setPlainText(message);
}

void RPNotifyMenu::SetNotificationType(NotificationType notify)
{
  mType = notify;
}

void RPNotifyMenu::SetNotificationKey(QString key)
{
  mKey = key;
}

void RPNotifyMenu::SetNotificationSender(int sender)
{
  mSender = sender;
}

void RPNotifyMenu::NotifyAccept()
{
  if(mType == NotificationType::PairRequest)
  {
    AOApplication::getInstance()->send_server_packet(DRPacket("PAIR", {QString::number(mSender), mKey}));
  }

  this->hide();
}

void RPNotifyMenu::NotifyDecline()
{
  this->hide();
}
