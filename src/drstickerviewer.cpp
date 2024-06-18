#include "drstickerviewer.h"

#include "aoapplication.h"
#include "drtheme.h"
#include "file_functions.h"

DRStickerViewer::DRStickerViewer(AOApplication *ao_app, QWidget *parent)
    : mk2::SpriteViewer(parent)
    , ao_app(ao_app)
    , m_hide_when_done(false)
{
  connect(this, SIGNAL(started()), this, SLOT(show()));
  connect(this, SIGNAL(finished()), this, SLOT(maybe_hide()));
}

DRStickerViewer::~DRStickerViewer()
{}

void DRStickerViewer::set_hide_when_done(bool p_enabled)
{
  if (m_hide_when_done == p_enabled)
  {
    return;
  }
  m_hide_when_done = p_enabled;
  if (!is_running())
  {
    maybe_hide();
  }
}

bool DRStickerViewer::set_theme_image(QString p_file_name)
{

  set_file_name(ao_app->get_theme_sprite_path(p_file_name, ""));
  return true;
}

bool DRStickerViewer::setFreeblockImage(QString p_file_name, QString t_fallblockName)
{
  QString l_FilePath = ao_app->GetFirstThemeSpritePath({p_file_name, t_fallblockName});
  set_file_name(l_FilePath);
  return true;
}

void DRStickerViewer::maybe_hide()
{
  if (m_hide_when_done)
  {
    hide();
  }
}

void DRStickerViewer::set_chatbox_image(QString p_chatbox_name, bool p_is_self, bool chatmessage_is_empty, QString pair)
{
  if(chatmessage_is_empty) return;
  QStringList targetChatboxes = {};

  bool usePairChatbox = false;
  if(pair == "left" || pair == "right") usePairChatbox = true;

  if(usePairChatbox)
  {
    if(p_is_self)
    {
      targetChatboxes.append("chatmed_self_" + pair);
    }
    targetChatboxes.append("chatmed_" + pair);
  }

  if(pair == "cg")
  {
    targetChatboxes.append("chatmed_cg");
  }

  if(p_is_self)
  {
    targetChatboxes.append("chatmed_self");
  }

  targetChatboxes.append("chatmed");



  QString l_target_file = ao_app->find_asset_path(ao_app->get_base_path() + "misc/" + p_chatbox_name + ".png");
  if (l_target_file.isEmpty())
  {

    for(QString chatbox_type : targetChatboxes)
    {
      l_target_file = ao_app->find_theme_asset_path(chatbox_type, animated_or_static_extensions());
      if (!l_target_file.isEmpty())
      {
        break;
      }
    }

  }

  if (l_target_file.isEmpty())
  {
    qWarning() << "warning: could not retrieve any chatbox image, will display blank";
  }
  QString old_filename = get_file_name();
  int current_frame = get_frame();
  set_file_name(l_target_file);
  restart(current_frame);

  DRTheme* drtheme = new DRTheme(ao_app);
  drtheme->InitTheme();
  this->hide();

}

void DRStickerViewer::set_variable_map(QMap<QString, QString> t_map)
{
  mVariables = t_map;
}

void DRStickerViewer::set_variable_string(QString t_path)
{
  mVariableString = t_path;
}

QString DRStickerViewer::getVariableString()
{
  return mVariableString;
}

QMap<QString, QString> DRStickerViewer::getVariableMap()
{
  return mVariables;
}
