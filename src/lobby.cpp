#include "lobby.h"

#include "aoapplication.h"
#include "aobutton.h"
#include "aoconfig.h"
#include "aoimagedisplay.h"
#include "commondefs.h"
#include "datatypes.h"
#include "debug_functions.h"
#include "drchatlog.h"
#include "drmasterclient.h"
#include "drpacket.h"
#include "drpather.h"
#include "drtextedit.h"
#include "theme.h"
#include "version.h"

#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QIcon>
#include <QInputDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPixmap>
#include <QProgressBar>
#include <QScopedPointer>
#include <QSettings>

Lobby::Lobby(AOApplication *p_ao_app) : QMainWindow()
{
  ao_app = p_ao_app;
  ao_config = new AOConfig(this);
  m_master_client = new DRMasterClient(this);

  this->setWindowTitle("Danganronpa Online");

  ui_background = new AOImageDisplay(this, ao_app);
  ui_hide_public_servers = new AOButton(this, ao_app);
  ui_hide_favorite_servers = new AOButton(this, ao_app);
  ui_refresh = new AOButton(this, ao_app);
  ui_toggle_favorite = new AOButton(this, ao_app);
  ui_connect = new AOButton(this, ao_app);
  ui_version = new DRTextEdit(this);
  ui_version->setFrameStyle(QFrame::NoFrame);
  ui_version->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui_version->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui_version->setReadOnly(true);
  ui_config_panel = new AOButton(this, ao_app);
  ui_server_list = new QListWidget(this);
  ui_player_count = new DRTextEdit(this);
  ui_player_count->setFrameStyle(QFrame::NoFrame);
  ui_player_count->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui_player_count->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui_player_count->setWordWrapMode(QTextOption::NoWrap);
  ui_player_count->setReadOnly(true);
  ui_description = new QTextBrowser(this);
  ui_description->setOpenExternalLinks(true);
  ui_description->setReadOnly(true);
  ui_chatbox = new DRChatLog(this);
  ui_chatbox->setOpenExternalLinks(true);
  ui_chatbox->setReadOnly(true);
  ui_loading_background = new AOImageDisplay(this, ao_app);
  ui_loading_text = new DRTextEdit(ui_loading_background);
  ui_progress_bar = new QProgressBar(ui_loading_background);
  ui_progress_bar->setMinimum(0);
  ui_progress_bar->setMaximum(100);
  ui_progress_bar->setStyleSheet("QProgressBar{ color: white; }");
  ui_cancel = new AOButton(ui_loading_background, ao_app);

  connect(ao_app, SIGNAL(reload_theme()), this, SLOT(update_widgets()));
  connect(ao_config, SIGNAL(server_advertiser_changed(QString)), m_master_client, SLOT(set_address(QString)));
  connect(m_master_client, SIGNAL(address_changed()), this, SLOT(request_advertiser_update()));
  connect(m_master_client, SIGNAL(motd_changed()), this, SLOT(update_motd()));
  connect(m_master_client, SIGNAL(server_list_changed()), this, SLOT(update_server_list()));

  connect(ui_hide_public_servers, SIGNAL(clicked()), this, SLOT(toggle_hide_public_servers()));
  connect(ui_hide_favorite_servers, SIGNAL(clicked()), this, SLOT(toggle_hide_favorite_servers()));
  connect(ui_refresh, SIGNAL(pressed()), this, SLOT(on_refresh_pressed()));
  connect(ui_refresh, SIGNAL(released()), this, SLOT(on_refresh_released()));
  connect(ui_toggle_favorite, SIGNAL(pressed()), this, SLOT(on_add_to_fav_pressed()));
  connect(ui_toggle_favorite, SIGNAL(released()), this, SLOT(on_add_to_fav_released()));
  connect(ui_connect, SIGNAL(pressed()), this, SLOT(on_connect_pressed()));
  connect(ui_connect, SIGNAL(released()), this, SLOT(on_connect_released()));
  connect(ui_config_panel, SIGNAL(pressed()), this, SLOT(on_config_pressed()));
  connect(ui_config_panel, SIGNAL(released()), this, SLOT(on_config_released()));
  connect(ui_server_list, SIGNAL(currentRowChanged(int)), this, SLOT(connect_to_server(int)));
  connect(ui_cancel, SIGNAL(clicked()), ao_app, SLOT(loading_cancelled()));

  update_widgets();
  load_settings();
  load_favorite_server_list();
  m_master_client->set_address(ao_config->server_advertiser());
  set_choose_a_server();
}

Lobby::~Lobby()
{
  save_settings();
}

DRServerInfoList Lobby::get_combined_server_list()
{
  return m_combined_server_list;
}

// sets images, position and size
void Lobby::update_widgets()
{
  pos_size_type f_lobby = ao_app->get_element_dimensions("lobby", LOBBY_DESIGN_INI);

  if (f_lobby.width < 0 || f_lobby.height < 0)
  {
    qWarning() << "W: did not find lobby width or height in " << LOBBY_DESIGN_INI;
    f_lobby.width = 517;
    f_lobby.height = 666;

    // Most common symptom of bad config files, missing assets, or misnamed
    // theme folder
    call_notice("It doesn't look like your client is set up correctly. This can be "
                "due to the following reasons: \n"
                "1. Check you downloaded and extracted the resources correctly from "
                "the DRO Discord including the large 'base' folder.\n"
                "2. If you did, check that the base folder is in the same folder "
                "where you launched Danganronpa Online from: " +
                DRPather::get_application_path() +
                "\n"
                "3. If it is there, check that your current theme folder exists in "
                "base/themes. According to base/config.ini, your current theme is " +
                ao_config->theme());
  }
  setWindowState(Qt::WindowNoState);
  resize(f_lobby.width, f_lobby.height);
  center_widget_to_screen(this);

  set_size_and_pos(ui_background, "lobby", LOBBY_DESIGN_INI, ao_app);
  ui_background->set_theme_image("lobbybackground.png");

  set_size_and_pos(ui_hide_public_servers, "public_servers", LOBBY_DESIGN_INI, ao_app);
  ui_hide_public_servers->set_image(m_hide_public_servers ? "publicservers_selected.png" : "publicservers.png");

  set_size_and_pos(ui_hide_favorite_servers, "favorites", LOBBY_DESIGN_INI, ao_app);
  ui_hide_favorite_servers->set_image(m_hide_favorite_servers ? "favorites_selected.png" : "favorites.png");

  set_size_and_pos(ui_refresh, "refresh", LOBBY_DESIGN_INI, ao_app);
  ui_refresh->set_image("refresh.png");

  set_size_and_pos(ui_toggle_favorite, "add_to_fav", LOBBY_DESIGN_INI, ao_app);
  ui_toggle_favorite->set_image("addtofav.png");

  set_size_and_pos(ui_connect, "connect", LOBBY_DESIGN_INI, ao_app);
  ui_connect->set_image("connect.png");

  set_size_and_pos(ui_version, "version", LOBBY_DESIGN_INI, ao_app);
  ui_version->setText("Version: " + get_version_string());

  set_size_and_pos(ui_config_panel, "config_panel", LOBBY_DESIGN_INI, ao_app);
  ui_config_panel->set_image_and_text("lobby_config_panel.png", "Config");
  if (ui_config_panel->isHidden() || ui_config_panel->size().isEmpty())
  {
    ui_config_panel->resize(64, 64);
    ui_config_panel->move(0, 0);
    ui_config_panel->show();
  }

  set_size_and_pos(ui_server_list, "server_list", LOBBY_DESIGN_INI, ao_app);
  ui_server_list->setStyleSheet("background-color: rgba(0, 0, 0, 0);"
                                "font: bold;");

  set_size_and_pos(ui_player_count, "player_count", LOBBY_DESIGN_INI, ao_app);
  ui_player_count->setStyleSheet("font: bold;"
                                 "color: white;"
                                 "qproperty-alignment: AlignCenter;");

  set_size_and_pos(ui_description, "description", LOBBY_DESIGN_INI, ao_app);
  ui_description->setStyleSheet("background-color: rgba(0, 0, 0, 0);"
                                "color: white;");

  set_size_and_pos(ui_chatbox, "chatbox", LOBBY_DESIGN_INI, ao_app);
  ui_chatbox->setReadOnly(true);
  ui_chatbox->setStyleSheet("QTextBrowser{background-color: rgba(0, 0, 0, 0);}");

  ui_loading_background->resize(this->width(), this->height());
  ui_loading_background->set_theme_image("loadingbackground.png");

  set_size_and_pos(ui_loading_text, "loading_label", LOBBY_DESIGN_INI, ao_app);
  ui_loading_text->setFont(QFont("Arial", 20, QFont::Bold));
  ui_loading_text->setReadOnly(true);
  ui_loading_text->setAlignment(Qt::AlignCenter);
  ui_loading_text->setFrameStyle(QFrame::NoFrame);
  ui_loading_text->setStyleSheet("background-color: rgba(0, 0, 0, 0);"
                                 "color: rgba(255, 128, 0, 255);");
  ui_loading_text->append("Loading");

  set_size_and_pos(ui_progress_bar, "progress_bar", LOBBY_DESIGN_INI, ao_app);
  set_size_and_pos(ui_cancel, "cancel", LOBBY_DESIGN_INI, ao_app);
  ui_cancel->setText("Cancel");

  ui_loading_background->hide();

  set_fonts();
  set_stylesheets();
  update_server_listing();
}

void Lobby::set_fonts()
{
  set_drtextedit_font(ui_player_count, "player_count", LOBBY_FONTS_INI, ao_app);
  set_font(ui_description, "description", LOBBY_FONTS_INI, ao_app);
  set_font(ui_chatbox, "chatbox", LOBBY_FONTS_INI, ao_app);
  set_drtextedit_font(ui_loading_text, "loading_text", LOBBY_FONTS_INI, ao_app);
  set_font(ui_server_list, "server_list", LOBBY_FONTS_INI, ao_app);
}

void Lobby::set_stylesheet(QWidget *widget, QString target_tag)
{
  QString f_file = "lobby_stylesheets.css";
  QString style_sheet_string = ao_app->get_stylesheet(target_tag, f_file);
  if (style_sheet_string != "")
    widget->setStyleSheet(style_sheet_string);
}

void Lobby::set_stylesheets()
{
  set_stylesheet(ui_player_count, "[PLAYER COUNT]");
  set_stylesheet(ui_description, "[DESCRIPTION]");
  set_stylesheet(ui_chatbox, "[CHAT BOX]");
  set_stylesheet(ui_loading_text, "[LOADING TEXT]");
  set_stylesheet(ui_server_list, "[SERVER LIST]");
}

void Lobby::show_loading_overlay()
{
  ui_loading_background->show();
}

void Lobby::hide_loading_overlay()
{
  ui_loading_background->hide();
}

void Lobby::set_loading_text(QString p_text)
{
  ui_loading_text->clear();
  ui_loading_text->setAlignment(Qt::AlignCenter);
  ui_loading_text->append(p_text);
}

DRServerInfo Lobby::get_selected_server()
{
  return m_current_server;
}

void Lobby::set_loading_value(int p_value)
{
  ui_progress_bar->setValue(p_value);
}

void Lobby::load_settings()
{
  QSettings l_ini(ao_app->get_base_file_path(BASE_SERVER_BROWSER_INI), QSettings::IniFormat);
  l_ini.setIniCodec("UTF-8");

  l_ini.beginGroup("filters");
  hide_public_servers(l_ini.value("hide_public", false).toBool());
  hide_favorite_servers(l_ini.value("hide_favorites", false).toBool());
  l_ini.endGroup();
}

void Lobby::save_settings()
{
  QSettings l_ini(ao_app->get_base_file_path(BASE_SERVER_BROWSER_INI), QSettings::IniFormat);
  l_ini.setIniCodec("UTF-8");

  l_ini.beginGroup("filters");
  l_ini.setValue("hide_public", m_hide_public_servers);
  l_ini.setValue("hide_favorites", m_hide_favorite_servers);
  l_ini.endGroup();
  l_ini.sync();
}

void Lobby::load_favorite_server_list()
{
  const QString l_file_path = ao_app->find_asset_path(ao_app->get_base_file_path(BASE_FAVORITE_SERVERS_INI));
  if (l_file_path.isEmpty())
  {
    load_legacy_favorite_server_list();
    return;
  }

  DRServerInfoList l_server_list;
  QSettings l_ini(l_file_path, QSettings::IniFormat);
  l_ini.setIniCodec("UTF-8");
  l_server_list.clear();
  for (const QString &i_group : l_ini.childGroups())
  {
    l_ini.beginGroup(i_group);
    DRServerInfo l_server;
    l_server.name = l_ini.value("name").toString();
    l_server.address = l_ini.value("address").toString();
    l_server.port = l_ini.value("port").toInt();
    l_server.favorite = true;
    l_server_list.append(std::move(l_server));
    l_ini.endGroup();
  }
  set_favorite_server_list(l_server_list);
}

void Lobby::load_legacy_favorite_server_list()
{
  DRServerInfoList l_server_list;
  QFile l_file(ao_app->get_base_file_path(BASE_SERVERLIST_TXT));
  if (l_file.open(QIODevice::ReadOnly))
  {
    QTextStream in(&l_file);
    while (!in.atEnd())
    {
      const QStringList l_contents = in.readLine().split(":");
      if (l_contents.length() < 3)
        continue;
      DRServerInfo f_server;
      f_server.address = l_contents.at(0);
      f_server.port = l_contents.at(1).toInt();
      f_server.name = l_contents.at(2);
      f_server.favorite = true;
      l_server_list.append(std::move(f_server));
    }
  }
  set_favorite_server_list(l_server_list);
}

void Lobby::save_favorite_server_list()
{
  QSettings l_ini(ao_app->get_base_file_path(BASE_FAVORITE_SERVERS_INI), QSettings::IniFormat);
  l_ini.setIniCodec("UTF-8");

  l_ini.clear();
  for (int i = 0; i < m_favorite_server_list.length(); ++i)
  {
    const DRServerInfo &i_server = m_favorite_server_list.at(i);
    l_ini.beginGroup(QString::number(i));
    l_ini.setValue("name", i_server.name);
    l_ini.setValue("address", i_server.address);
    l_ini.setValue("port", i_server.port);
    l_ini.endGroup();
  }
  l_ini.sync();
}

void Lobby::request_advertiser_update()
{
  m_master_client->request_motd();
  m_master_client->request_server_list();
}

void Lobby::update_motd()
{
  ui_chatbox->append_html(m_master_client->motd());
}

void Lobby::update_server_list()
{
  m_server_list = m_master_client->server_list();
  update_combined_server_list();
  emit server_list_changed();
}

void Lobby::set_favorite_server_list(DRServerInfoList p_server_list)
{
  m_favorite_server_list = p_server_list;
  update_combined_server_list();
  emit favorite_server_list_changed();
}

void Lobby::update_combined_server_list()
{
  m_combined_server_list = m_favorite_server_list + m_server_list;
  update_server_listing();
}

void Lobby::update_server_listing()
{
  ui_server_list->clear();
  const QIcon l_favorite_icon = QPixmap(ao_app->find_theme_asset_path("favorite_server.png"));
  const QBrush l_favorite_color = ao_app->get_color("favorite_server_color", LOBBY_DESIGN_INI);
  for (const DRServerInfo &l_server : qAsConst(m_combined_server_list))
  {
    QListWidgetItem *l_server_item = new QListWidgetItem;
    ui_server_list->addItem(l_server_item);
    l_server_item->setText(l_server.name);
    if (l_server.favorite)
    {
      l_server_item->setIcon(l_favorite_icon);
      l_server_item->setBackground(l_favorite_color);
    }
  }
  filter_server_listing();
}

void Lobby::filter_server_listing()
{
  for (int i = 0; i < ui_server_list->count(); ++i)
  {
    QListWidgetItem *l_server_item = ui_server_list->item(i);
    l_server_item->setHidden(m_combined_server_list.at(i).favorite ? m_hide_favorite_servers : m_hide_public_servers);
  }
  select_current_server();
}

void Lobby::select_current_server()
{
  for (int i = 0; i < ui_server_list->count(); ++i)
  {
    QListWidgetItem *l_item = ui_server_list->item(i);
    if (l_item->text() == m_current_server.name)
    {
      ui_server_list->scrollToItem(l_item);
      ui_server_list->setCurrentItem(l_item);
      ui_server_list->setFocus();
      break;
    }
  }
}

void Lobby::hide_public_servers(bool p_on)
{
  if (m_hide_public_servers == p_on)
    return;
  m_hide_public_servers = p_on;
  ui_hide_public_servers->set_image(m_hide_public_servers ? "publicservers_selected.png" : "publicservers.png");
  filter_server_listing();
}

void Lobby::toggle_hide_public_servers()
{
  hide_public_servers(!m_hide_public_servers);
}

void Lobby::hide_favorite_servers(bool p_on)
{
  if (m_hide_favorite_servers == p_on)
    return;
  m_hide_favorite_servers = p_on;
  ui_hide_favorite_servers->set_image(m_hide_favorite_servers ? "favorites_selected.png" : "favorites.png");
  filter_server_listing();
}

void Lobby::toggle_hide_favorite_servers()
{
  hide_favorite_servers(!m_hide_favorite_servers);
}

void Lobby::on_refresh_pressed()
{
  ui_refresh->set_image("refresh_pressed.png");
}

void Lobby::on_refresh_released()
{
  ui_refresh->set_image("refresh.png");
  m_master_client->request_server_list();
}

void Lobby::on_add_to_fav_pressed()
{
  ui_toggle_favorite->set_image("addtofav_pressed.png");
}

void Lobby::on_add_to_fav_released()
{
  ui_toggle_favorite->set_image("addtofav.png");
  DRServerInfoList l_new_list = m_favorite_server_list;
  if (m_current_server.favorite)
  {
    l_new_list.removeAll(m_current_server);
  }
  else if (!m_favorite_server_list.contains(m_current_server))
  {
    m_current_server.favorite = true;

    const QString l_new_name =
        QInputDialog::getText(this, windowTitle(), "Name", QLineEdit::Normal, m_current_server.name);
    if (!l_new_name.isEmpty())
      m_current_server.name = l_new_name;

    l_new_list.append(m_current_server);
  }
  set_favorite_server_list(l_new_list);
  save_favorite_server_list();
}

void Lobby::on_connect_pressed()
{
  ui_connect->set_image("connect_pressed.png");
}

void Lobby::on_connect_released()
{
  const VersionStatus l_status = ao_app->get_server_client_version_status();
  if (l_status != VersionStatus::Ok)
  {
    QString l_reason;
    switch (l_status)
    {
    case VersionStatus::NotCompatible:
      l_reason = "The server did not report any client version.";
      break;
    case VersionStatus::ServerOutdated:
      l_reason = QString("The server is outdated.<br />(Server version: <b>%1</b>, expected version: <b>%2</b>)")
                     .arg(ao_app->get_server_client_version().to_string(), get_version_number().to_string());
      break;
    case VersionStatus::ClientOutdated:
      l_reason = QString("Your client is outdated.<br />(Client version: <b>%1</b>, expected version: <b>%2</b>)")
                     .arg(get_version_number().to_string(), ao_app->get_server_client_version().to_string());
      break;
    default:
      break;
    }

    call_warning("You are connecting to an <b>incompatible</b> DRO server.<br /><br />Reason: " + l_reason +
                 "<br /><br />"
                 "The client may not work properly, if at all.");
  }

  ui_connect->set_image("connect.png");
  ao_app->send_server_packet(DRPacket("askchaa"));
}

void Lobby::on_config_pressed()
{
  ui_config_panel->set_image("lobby_config_panel_pressed.png");
}

void Lobby::on_config_released()
{
  ui_config_panel->set_image("lobby_config_panel.png");
  ao_app->toggle_config_panel();
}

void Lobby::connect_to_server(int p_row)
{
  if (p_row == -1)
    return;

  const DRServerInfo l_prev_server = std::move(m_current_server);
  m_current_server = m_combined_server_list.at(p_row);
  if (l_prev_server != m_current_server)
  {
    ui_player_count->setText("Connecting...");
    ui_description->setHtml("Connecting to " + m_current_server.name + "...");
    ao_app->connect_to_server(m_current_server);
  }
}

void Lobby::set_choose_a_server()
{
  ui_player_count->setText(nullptr);
  ui_description->setHtml(tr("Choose a server."));
}

void Lobby::set_player_count(int players_online, int max_players)
{
  const QString f_string = "Connected: " + QString::number(players_online) + "/" + QString::number(max_players);
  ui_player_count->setText(f_string);
  ui_player_count->setAlignment(Qt::AlignHCenter);

  QString l_text = m_current_server.description.toHtmlEscaped();
  const QRegExp l_regex("(https?://[^\\s/$.?#].[^\\s]*)");
  if (l_text.contains(l_regex))
    l_text.replace(l_regex, "<a href=\"\\1\">\\1</a>");
  ui_description->setHtml(l_text.replace("\n", "<br />"));
}
