#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "datatypes.h"

class AOApplication;

#include <QAbstractSocket>
#include <QObject>

class QTcpSocket;
class QTimer;

class NetworkManager : public QObject
{
  Q_OBJECT

public:
  static const QString MASTER_HOST;
  static const int MASTER_PORT;
  static const int MASTER_RECONNECT_DELAY;

  NetworkManager(AOApplication *parent);
  ~NetworkManager();

  void connect_to_master();
  void connect_to_master_nosrv();
  void connect_to_server(server_type p_server);
  void disconnect_from_server();

public slots:
  void ship_ms_packet(QString p_packet);
  void ship_server_packet(QString p_packet);

signals:
  void ms_connect_finished(bool success, bool will_retry);

private:
  AOApplication *ao_app = nullptr;
  QTcpSocket *ms_socket = nullptr;
  QTcpSocket *server_socket = nullptr;
  QTimer *ms_reconnect_timer = nullptr;

  bool ms_partial_packet = false;
  QString ms_temp_packet;

  bool partial_packet = false;
  QString temp_packet;

private slots:
  void handle_ms_packet();
  void handle_server_packet();
  void on_ms_nosrv_connect_success();
  void on_ms_socket_error(QAbstractSocket::SocketError error);
  void retry_ms_connect();
};

#endif // NETWORKMANAGER_H
