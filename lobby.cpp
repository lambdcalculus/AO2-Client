#include "lobby.h"

#include "path_functions.h"
#include "text_file_functions.h"
#include "global_variables.h"
#include "debug_functions.h"
#include "aoapplication.h"
#include "networkmanager.h"

#include <QDebug>

Lobby::Lobby(AOApplication *parent) : QMainWindow()
{
  ao_app = parent;

  this->setWindowTitle("Attorney Online 2");
  this->resize(m_lobby_width, m_lobby_height);

  ui_background = new AOImage(this);
  ui_public_servers = new AOButton(this);
  ui_favorites = new AOButton(this);
  ui_refresh = new AOButton(this);
  ui_add_to_fav = new AOButton(this);
  ui_connect = new AOButton(this);
  ui_about = new AOButton(this);
  ui_server_list = new QListWidget(this);
  ui_player_count = new QLabel(this);
  ui_description = new QPlainTextEdit(this);

  connect(ui_public_servers, SIGNAL(clicked()), this, SLOT(on_public_servers_clicked()));
  connect(ui_favorites, SIGNAL(clicked()), this, SLOT(on_favorites_clicked()));
  connect(ui_refresh, SIGNAL(pressed()), this, SLOT(on_refresh_pressed()));
  connect(ui_refresh, SIGNAL(released()), this, SLOT(on_refresh_released()));
  connect(ui_add_to_fav, SIGNAL(pressed()), this, SLOT(on_add_to_fav_pressed()));
  connect(ui_add_to_fav, SIGNAL(released()), this, SLOT(on_add_to_fav_released()));
  connect(ui_connect, SIGNAL(pressed()), this, SLOT(on_connect_pressed()));
  connect(ui_connect, SIGNAL(released()), this, SLOT(on_connect_released()));

  connect(ui_about, SIGNAL(clicked()), this, SLOT(on_about_clicked()));

  set_widgets();
}

Lobby::~Lobby()
{
  delete ui_background;
}

//sets images, position and size
void Lobby::set_widgets()
{
  //global to efficiently set images on button presses
  g_user_theme = get_user_theme();

  ui_background->set_image("lobbybackground.png");
  ui_background->move(0, 0);
  ui_background->resize(m_lobby_width, m_lobby_height);

  ui_public_servers->set_image("publicservers_selected.png");
  ui_public_servers->move(46, 88);
  ui_public_servers->resize(114, 30);

  ui_favorites->set_image("favorites.png");
  ui_favorites->move(164, 88);
  ui_favorites->resize(114, 30);

  ui_refresh->set_image("refresh.png");
  ui_refresh->move(56, 381);
  ui_refresh->resize(132, 28);

  ui_add_to_fav->set_image("addtofav.png");
  ui_add_to_fav->move(194, 381);
  ui_add_to_fav->resize(132, 28);

  ui_connect->set_image("connect.png");
  ui_connect->move(332, 381);
  ui_connect->resize(132, 28);

  ui_about->set_image("about.png");
  ui_about->move(428, 1);
  ui_about->resize(88, 21);

  ui_server_list->move(20, 125);
  ui_server_list->resize(286, 240);
  ui_server_list->setStyleSheet("background-color: rgba(0, 0, 0, 0);"
                                "font: bold;");

  ui_player_count->move(336, 91);
  ui_player_count->resize(173, 16);
  ui_player_count->setText("Offline");
  ui_player_count->setStyleSheet("font: bold;"
                                 "color: white;"
                                 "qproperty-alignment: AlignCenter;");

  ui_description->move(337, 109);
  ui_description->resize(173, 245);
  ui_description->setStyleSheet("background-color: rgba(0, 0, 0, 0);"
                                "color: white;");
}

void Lobby::on_public_servers_clicked()
{
  ui_public_servers->set_image("publicservers_selected.png");
  ui_favorites->set_image("favorites.png");

  //T0D0: clear server list and show public servers
}

void Lobby::on_favorites_clicked()
{
  ui_favorites->set_image("favorites_selected.png");
  ui_public_servers->set_image("publicservers.png");

  //T0D0: clear server list and show favorites from serverlist.txt
}

void Lobby::on_refresh_pressed()
{
  ui_refresh->set_image("refresh_pressed.png");
}

void Lobby::on_refresh_released()
{
  ui_refresh->set_image("refresh.png");

  AOPacket *f_packet = new AOPacket("ALL#%");

  ao_app->net_manager->send_ms_packet(f_packet);

  delete f_packet;
}

void Lobby::on_add_to_fav_pressed()
{
  ui_add_to_fav->set_image("addtofav_pressed.png");
}

void Lobby::on_add_to_fav_released()
{
  ui_add_to_fav->set_image("addtofav.png");

  //T0D0, add selected element to serverlist.txt
}

void Lobby::on_connect_pressed()
{
  ui_connect->set_image("connect_pressed.png");
}

void Lobby::on_connect_released()
{
  ui_connect->set_image("connect.png");

  //T0D0: call ao_app to initialize loading sequence
}

void Lobby::on_about_clicked()
{
  //T0D0: add something real here
  call_error("YEBOIIII");
}

void Lobby::list_servers()
{
  for (server_type i_server : ao_app->server_list)
  {
    ui_server_list->addItem(i_server.name);
  }
}
