#include "aoscene.h"
// src
#include "courtroom.h"
#include "file_functions.h"
// qt
#include <QDebug>
#include <QMovie>

AOScene::AOScene(QWidget *parent, AOApplication *p_ao_app) : QLabel(parent), ao_app(p_ao_app)
{
    m_movie = new QMovie(this);
    setMovie(m_movie);
}

void AOScene::set_image(QString p_image)
{
    QString target_path = ao_app->get_default_background_path() + p_image;

    // background specific path
    QString background_path = ao_app->get_background_path() + p_image;

    for (auto &ext : QVector<QString>{".webp", ".apng", ".gif", ".png"})
    {
        QString full_background_path = background_path + ext;

        if (file_exists(full_background_path))
        {
            target_path = full_background_path;
            break;
        }
    }

    // do not update the movie if we're using the same file
    if (m_movie->fileName() == target_path)
        return;
    m_movie->stop();
    m_movie->setFileName(target_path);
    m_movie->start();
}

void AOScene::combo_resize(QSize p_size)
{
    resize(p_size);
    m_movie->stop();
    m_movie->setScaledSize(p_size);
    m_movie->start();
}
