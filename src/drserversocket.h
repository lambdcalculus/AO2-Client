#pragma once

#include "datatypes.h"
#include "drpacket.h"

#include <QAbstractSocket>
#include <QObject>

class QTcpSocket;
class QTimer;

class DRServerSocket : public QObject
{
  Q_OBJECT

public:
  DRServerSocket(QObject *parent = nullptr);

  bool is_connected() const;

public slots:
  void connect_to_server(DRServerInfo server);

  void disconnect_from_server();

  void send_packet(DRPacket packet);

signals:
  void connected_to_server();
  void connecting_to_server();
  void disconnected_from_server();
  void packet_received(DRPacket);
  void socket_error(QString);

private:
  static const int CONNECTING_DELAY;

  DRServerInfo m_server;
  QTcpSocket *m_socket = nullptr;
  QTimer *m_connecting_timer = nullptr;
  bool m_connected = false;
  QString m_buffer;

private slots:
  void _p_update_state(QAbstractSocket::SocketState);
  void _p_check_socket_error();
  void _p_read_socket();
};
