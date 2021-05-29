#include "aoapplication.h"

#include "aopacket.h"
#include "debug_functions.h"
#include "drserversocket.h"
#include "hardware_functions.h"
#include "lobby.h"
#include "version.h"

#include <QDebug>

void AOApplication::send_master_packet(AOPacket *p_packet)
{
  qDebug().noquote() << "M/S:" << p_packet->to_string();
  m_master_socket->send_packet(*p_packet);
}

void AOApplication::request_server_list()
{
  m_master_socket->send_packet(AOPacket("ALL", {}));
}

void AOApplication::_p_send_master_handshake()
{
#ifndef QT_DEBUG
  send_master_packet(new AOPacket("ALL#%"));
#endif
}

void AOApplication::_p_handle_master_error(QString p_error)
{
  if (!is_lobby_constructed)
    return;
  m_lobby->append_error(p_error);
}

void AOApplication::_p_handle_master_packet(AOPacket p_packet)
{
  p_packet.net_decode();

  QString l_header = p_packet.get_header();
  QStringList l_content = p_packet.get_contents();

  if (l_header != "CHECK")
    qDebug().noquote() << "M/R:" << p_packet.to_string();

  if (l_header == "ALL")
  {
    m_server_list.clear();

    for (const QString &i_string : p_packet.get_contents())
    {
      server_type f_server;
      QStringList sub_contents = i_string.split("&");

      if (sub_contents.size() < 4)
      {
        qDebug() << "W: malformed packet";
        continue;
      }

      f_server.name = sub_contents.at(0);
      f_server.desc = sub_contents.at(1);
      f_server.ip = sub_contents.at(2);
      f_server.port = sub_contents.at(3).toInt();

      m_server_list.append(f_server);
    }

    if (is_lobby_constructed)
    {
      m_lobby->list_servers();
    }
  }
  else if (l_header == "CT")
  {
    QString f_name, f_message;

    if (l_content.size() == 1)
    {
      f_name = "";
      f_message = l_content.at(0);
    }
    else if (l_content.size() >= 2)
    {
      f_name = l_content.at(0);
      f_message = l_content.at(1);
    }
    else
      return;

    if (is_lobby_constructed)
    {
      m_lobby->append_chatmessage(f_name, f_message);
    }
  }
  else if (l_header == "AO2CHECK")
  {
    send_master_packet(new AOPacket("ID#DRO#" + get_version_string() + "#%"));
    send_master_packet(new AOPacket("HI#" + get_hdid() + "#%"));

    if (l_content.size() < 1)
      return;

    QStringList version_contents = l_content.at(0).split(".");

    if (version_contents.size() < 3)
      return;

    int f_release = version_contents.at(0).toInt();
    int f_major = version_contents.at(1).toInt();
    int f_minor = version_contents.at(2).toInt();

    if (get_release_version() > f_release)
      return;
    else if (get_release_version() == f_release)
    {
      if (get_major_version() > f_major)
        return;
      else if (get_major_version() == f_major)
      {
        if (get_minor_version() >= f_minor)
          return;
      }
    }

    call_notice("Outdated version! Your version: " + get_version_string() +
                "\nPlease go to aceattorneyonline.com to update.");
    destruct_courtroom();
    destruct_lobby();
  }
}
