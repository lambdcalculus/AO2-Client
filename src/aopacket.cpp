#include "aopacket.h"

#include <QDebug>

AOPacket::AOPacket(QString p_packet_string)
{
  QStringList l_contents = p_packet_string.split("#");

  m_header = l_contents.at(0);

  for (int n_string = 1; n_string < l_contents.size() - 1; ++n_string)
  {
    m_contents.append(l_contents.at(n_string));
  }
}

AOPacket::AOPacket(QString p_header, const QStringList &p_contents)
{
  m_header = p_header;
  m_contents = p_contents;
}

QString AOPacket::get_header()
{
  return m_header;
}

QStringList &AOPacket::get_contents()
{
  return m_contents;
}

QString AOPacket::to_string()
{
  QString r_data;
  for (const QString &i_value : qAsConst(m_contents))
  {
    if (!r_data.isEmpty())
      r_data += "#";
    r_data += i_value;
  }
  return m_header + "#" + r_data + (r_data.isEmpty() ? "%" : "#%");
}

void AOPacket::net_encode()
{
  for (QString &i_value : m_contents)
    i_value.replace("#", "<num>").replace("%", "<percent>").replace("$", "<dollar>").replace("&", "<and>");
}

void AOPacket::net_decode()
{
  for (QString &i_value : m_contents)
    i_value.replace("<num>", "#").replace("<percent>", "%").replace("<dollar>", "$").replace("<and>", "&");
}
