#include "drstickerviewer.h"

#include "aoapplication.h"
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

void DRStickerViewer::set_theme_image(QString p_file_name)
{
  set_file_name(ao_app->get_theme_sprite_path(p_file_name, ""));
}

void DRStickerViewer::maybe_hide()
{
  if (m_hide_when_done)
  {
    hide();
  }
}

void DRStickerViewer::set_chatbox_image(QString p_chatbox_name, bool p_is_self, bool chatmessage_is_empty)
{
  if(chatmessage_is_empty) return;
  QString l_target_file = ao_app->find_asset_path(ao_app->get_base_path() + "misc/" + p_chatbox_name + ".png");
  if (l_target_file.isEmpty())
  {
    l_target_file = ao_app->find_theme_asset_path("chatmed", animated_or_static_extensions());

    if (p_is_self)
    {
      const QString l_self_file = ao_app->find_theme_asset_path("chatmed_self", animated_or_static_extensions());
      if (!l_self_file.isEmpty())
      {
        l_target_file = l_self_file;
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
  if(old_filename != l_target_file) restart(current_frame);

}
