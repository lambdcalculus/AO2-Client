QT += core gui widgets uitools network multimedia multimediawidgets

CONFIG += c++17

TEMPLATE = app
VERSION = 1.1.0.0
TARGET = dro-client

RC_ICONS = icon.ico

INCLUDEPATH += $$PWD/include $$PWD/3rd $$PWD/3rd/include
DEPENDPATH += $$PWD/include

DEFINES += DRO_ACKMS
CONFIG(debug, debug|release):DEFINES += DR_DEV

HEADERS += \
  include/draudiotrackmetadata.h \
  include/drscenemovie.h \
  include/drsplashmovie.h \
  include/aoapplication.h \
  include/aoblipplayer.h \
  include/aobutton.h \
  include/aocharbutton.h \
  include/aoconfig.h \
  include/aoconfigpanel.h \
  include/aoemotebutton.h \
  include/aoguiloader.h \
  include/aoimagedisplay.h \
  include/aolabel.h \
  include/aolineedit.h \
  include/aomusicplayer.h \
  include/aonotearea.h \
  include/aonotepad.h \
  include/aonotepicker.h \
  include/aoobject.h \
  include/aopixmap.h \
  include/aosfxplayer.h \
  include/aoshoutplayer.h \
  include/aosystemplayer.h \
  include/aotimer.h \
  include/commondefs.h \
  include/courtroom.h \
  include/datatypes.h \
  include/debug_functions.h \
  include/draudio.h \
  include/draudiodevice.h \
  include/draudioengine.h \
  include/draudioengine_p.h \
  include/draudioerror.h \
  include/draudiostream.h \
  include/draudiostreamfamily.h \
  include/drcharactermovie.h \
  include/drchatlog.h \
  include/dreffectmovie.h \
  include/drmovie.h \
  include/drpacket.h \
  include/drpather.h \
  include/drserversocket.h \
  include/drshoutmovie.h \
  include/drstickermovie.h \
  include/drtextedit.h \
  include/drdiscord.h \
  include/drvideoscreen.h \
  include/file_functions.h \
  include/hardware_functions.h \
  include/lobby.h \
  include/logger.h \
  include/misc_functions.h \
  include/theme.h \
  include/utils.h \
  include/version.h

SOURCES += \
  src/aoapplication.cpp \
  src/aoblipplayer.cpp \
  src/aobutton.cpp \
  src/aocharbutton.cpp \
  src/aoconfig.cpp \
  src/aoconfigpanel.cpp \
  src/aoemotebutton.cpp \
  src/aoguiloader.cpp \
  src/aoimagedisplay.cpp \
  src/aolabel.cpp \
  src/aolineedit.cpp \
  src/aomusicplayer.cpp \
  src/aonotearea.cpp \
  src/aonotepad.cpp \
  src/aonotepicker.cpp \
  src/aoobject.cpp \
  src/aopixmap.cpp \
  src/aosfxplayer.cpp \
  src/aoshoutplayer.cpp \
  src/aosystemplayer.cpp \
  src/aotimer.cpp \
  src/audio_functions.cpp \
  src/charselect.cpp \
  src/commondefs.cpp \
  src/courtroom.cpp \
  src/courtroom_character.cpp \
  src/courtroom_sfx.cpp \
  src/courtroom_widgets.cpp \
  src/datatypes.cpp \
  src/debug_functions.cpp \
  src/draudio.cpp \
  src/draudiodevice.cpp \
  src/draudioengine.cpp \
  src/draudioengine_p.cpp \
  src/draudioerror.cpp \
  src/draudiostream.cpp \
  src/draudiostreamfamily.cpp \
  src/draudiotrackmetadata.cpp \
  src/drcharactermovie.cpp \
  src/drchatlog.cpp \
  src/dreffectmovie.cpp \
  src/drmovie.cpp \
  src/drpacket.cpp \
  src/drpather.cpp \
  src/drscenemovie.cpp \
  src/drserversocket.cpp \
  src/drshoutmovie.cpp \
  src/drsplashmovie.cpp \
  src/drstickermovie.cpp \
  src/drtextedit.cpp \
  src/drdiscord.cpp \
  src/drvideoscreen.cpp \
  src/emotes.cpp \
  src/file_functions.cpp \
  src/hardware_functions.cpp \
  src/lobby.cpp \
  src/logger.cpp \
  src/main.cpp \
  src/master_socket.cpp \
  src/misc_functions.cpp \
  src/path_functions.cpp \
  src/server_socket.cpp \
  src/text_file_functions.cpp \
  src/theme.cpp \
  src/utils.cpp \
  src/version.cpp

# 1. You need to get BASS and put the x86 bass DLL/headers in the project root folder
#    AND the compilation output folder. If you want a static link, you'll probably
#    need the .lib file too. MinGW-GCC is really finicky finding BASS, it seems.
# 2. You need to compile the Discord Rich Presence SDK separately and add the lib/headers
#    in the same way as BASS. Discord RPC uses CMake, which does not play nicely with
#    QMake, so this step must be manual.
LIBS += -L$$PWD/3rd/$${QMAKE_HOST.arch} -lbass -lbassopus -ldiscord-rpc

RESOURCES += \
  res.qrc

DISTFILES +=

FORMS += \
  res/ui/config_panel.ui

# Mac stuff
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
ICON = icon.icns
