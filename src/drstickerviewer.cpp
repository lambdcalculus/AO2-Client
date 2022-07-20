#include "drstickerviewer.h"

#include "aoapplication.h"

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

void DRStickerViewer::set_hide_when_done(bool p_on)
{
  if (m_hide_when_done == p_on)
  {
    return;
  }
  m_hide_when_done = p_on;
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
