#include "drserversocket.h"
#include "qabstractsocket.h"

#include <QTcpSocket>
#include <QTimer>

const int DRServerSocket::CONNECTING_DELAY = 5000;

namespace
{
QString drFormatServerInfo(const DRServerInfo &server)
{
  const QString l_server_info = server.to_info();
  return !l_server_info.isEmpty() ? QString("<%1>").arg(l_server_info) : nullptr;
}
} // namespace

DRServerSocket::DRServerSocket(QObject *p_parent)
    : QObject(p_parent)
{
  m_socket = new QTcpSocket(this);
  m_connecting_timer = new QTimer(this);

  m_connecting_timer->setSingleShot(true);
  m_connecting_timer->setInterval(CONNECTING_DELAY);

  connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_p_check_socket_error()));
  connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
          SLOT(_p_update_state(QAbstractSocket::SocketState)));
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(_p_read_socket()));

  connect(m_connecting_timer, SIGNAL(timeout()), this, SLOT(disconnect_from_server()));
  m_socket->close();
}

bool DRServerSocket::is_connected() const
{
  return m_socket->state() == QTcpSocket::ConnectedState;
}

void DRServerSocket::connect_to_server(DRServerInfo p_server)
{
  disconnect_from_server();
  m_server = p_server;
  m_socket->connectToHost(p_server.address, p_server.port);
}

void DRServerSocket::disconnect_from_server()
{
  m_socket->close();
  m_socket->abort();
  m_buffer.clear();
}

void DRServerSocket::send_packet(DRPacket p_packet)
{
  if (!is_connected())
  {
    const QString l_server_info = m_server.to_info();
    qWarning().noquote()
        << QString("Failed to send packet; not connected to server%1").arg(drFormatServerInfo(m_server));
    return;
  }
  m_socket->write(p_packet.to_string(true).toUtf8());
}

void DRServerSocket::_p_update_state(QAbstractSocket::SocketState p_state)
{
  switch (p_state)
  {
  case QAbstractSocket::ConnectingState:
    m_connecting_timer->start();
    Q_EMIT connecting_to_server();
    break;

  case QAbstractSocket::ConnectedState:
    m_connecting_timer->stop();
    m_connected = true;
    Q_EMIT connected_to_server();
    break;

  case QAbstractSocket::UnconnectedState:
    m_connecting_timer->stop();
    m_connected = false;
    Q_EMIT disconnected_from_server();
    break;

  default:
    break;
  }
}

void DRServerSocket::_p_check_socket_error()
{
  const QString l_error = QString("Server%1 error: %2").arg(drFormatServerInfo(m_server), m_socket->errorString());
  qWarning().noquote() << l_error;
  Q_EMIT socket_error(l_error);
}

void DRServerSocket::_p_read_socket()
{
  m_buffer += QString::fromUtf8(m_socket->readAll());
  QStringList l_raw_packet_list = m_buffer.split("#%", DR::KeepEmptyParts);
  m_buffer = l_raw_packet_list.takeLast();
  for (const QString &i_raw_packet : l_raw_packet_list)
  {
    QStringList l_raw_data_list = i_raw_packet.split("#");
    const QString l_header = l_raw_data_list.takeFirst();
    for (QString &i_raw_data : l_raw_data_list)
      i_raw_data = DRPacket::decode(i_raw_data);
    Q_EMIT packet_received(DRPacket(l_header, l_raw_data_list));
  }
}
