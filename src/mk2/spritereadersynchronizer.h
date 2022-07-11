/**************************************************************************
**
** mk2
** Copyright (C) 2022 Tricky Leifa
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Affero General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Affero General Public License for more details.
**
** You should have received a copy of the GNU Affero General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#pragma once

#include "mk2/spritereader.h"

#include <QObject>

namespace mk2
{
class SpriteReaderSynchronizer : public QObject
{
  Q_OBJECT

public:
  SpriteReaderSynchronizer(QObject *parent = nullptr);
  ~SpriteReaderSynchronizer();

  int get_threshold() const;

  QVector<mk2::SpriteReader::ptr> get_reader_list() const;

  bool is_waiting() const;

  bool is_finished() const;

public slots:
  void set_threshold(int percent);

  void add(mk2::SpriteReader::ptr reader);

  void clear();

  void start();

signals:
  void finished();

private:
  QVector<mk2::SpriteReader::ptr> m_reader_list;
  bool m_waiting;
  bool m_finished;
  int m_threshold;

private slots:
  void _p_check_progress();
};
} // namespace mk2
