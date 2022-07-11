#include "drmovie.h"

DRMovie::DRMovie(QWidget *parent)
    : SpriteViewer{parent}
    , m_hide_when_done{false}
    , m_mirrored{false}
{
  connect(this, SIGNAL(started()), this, SLOT(show()));
  connect(this, SIGNAL(finished()), this, SIGNAL(done()));
  connect(this, SIGNAL(finished()), this, SLOT(_p_update_visibility()));
}

DRMovie::~DRMovie()
{}

QString DRMovie::file_name()
{
  return get_file_name();
}

void DRMovie::set_hide_on_done(bool p_on)
{
  m_hide_when_done = p_on;
}

void DRMovie::set_mirrored(bool p_on)
{
  set_mirror(p_on);
}

void DRMovie::_p_update_visibility()
{
  if (m_hide_when_done)
  {
    hide();
  }
}
