#include "drposition.h"

#include <QDebug>
#include <QSettings>

const QMap<QString, DRPosition> DRPositionReader::READONLY_POSITIONS{
    {
        "wit",
        DRPosition("witnessempty", "stand"),
    },
    {
        "def",
        DRPosition("defenseempty", "defensedesk"),
    },
    {
        "pro",
        DRPosition("prosecutorempty", "prosecutiondesk"),
    },
    {
        "jud",
        DRPosition("judgestand", "judgedesk"),
    },
    {
        "hld",
        DRPosition("helperstand", "helperdesk"),
    },
    {
        "hlp",
        DRPosition("prohelperstand", "prohelperdesk"),
    },
};

DRPosition::DRPosition()
{}

DRPosition::DRPosition(QString p_back, QString p_front) : m_back(p_back), m_front(p_front)
{}

DRPosition::~DRPosition()
{}

QString DRPosition::get_back()
{
  return m_back;
}

QString DRPosition::get_front()
{
  return m_front;
}

void DRPosition::set_back(QString p_back)
{
  m_back = p_back;
}

void DRPosition::set_front(QString p_front)
{
  m_front = p_front;
}

DRPositionReader::DRPositionReader(QObject *parent) : QObject(parent)
{}

DRPositionReader::~DRPositionReader()
{}

DRPosition DRPositionReader::get_position(QString p_id)
{
  p_id = p_id.toLower();
  return m_positions.contains(p_id) ? m_positions.value(p_id) : READONLY_POSITIONS.value(p_id);
}

void DRPositionReader::load_file(QString p_filename)
{
  QSettings l_settings(p_filename, QSettings::IniFormat);
  l_settings.setIniCodec("UTF-8");
  if (l_settings.status() != QSettings::NoError)
  {
    qWarning() << "[Position Reader]"
               << "warning:"
               << "could not load positions.ini file";
    return;
  }

  m_positions.clear();
  const QStringList l_group_list = l_settings.childGroups();
  for (const QString &i_group : l_group_list)
  {
    const QString l_lower_group = i_group.toLower();
    l_settings.beginGroup(i_group);
    const QString l_back = l_settings.value("back").toString();
    const QString l_front = l_settings.value("front").toString();
    m_positions.insert(l_lower_group, DRPosition(l_back, l_front));
    l_settings.endGroup();
  }
}
