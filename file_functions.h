#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <QString>
#include <QVector>

bool file_exists(QString file_path);
bool file_exists(QString file_path, QVector<QString> p_exts);
bool dir_exists(QString file_path);

#endif // FILE_FUNCTIONS_H
