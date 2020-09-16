QT += core gui widgets uitools multimedia network

CONFIG += c++11

TEMPLATE = app
VERSION = 1.0.0.0
TARGET = dro-client

RC_ICONS = logo.ico

INCLUDEPATH += $$PWD/include $$PWD/3rd
DEPENDPATH += $$PWD/include

HEADERS += \
  include/aoabstractplayer.h \
  include/aoapplication.h \
  include/aobasshandle.h \
  include/aoblipplayer.h \
  include/aobutton.h \
  include/aocharbutton.h \
  include/aocharmovie.h \
  include/aoconfig.h \
  include/aoconfigpanel.h \
  include/aoemotebutton.h \
  include/aoevidencebutton.h \
  include/aoevidencedescription.h \
  include/aoevidencedisplay.h \
  include/aoexception.h \
  include/aoguiloader.h \
  include/aoimage.h \
  include/aolabel.h \
  include/aolineedit.h \
  include/aomovie.h \
  include/aomusicplayer.h \
  include/aonotearea.h \
  include/aonotepad.h \
  include/aonotepicker.h \
  include/aopacket.h \
  include/aopixmap.h \
  include/aoscene.h \
  include/aosfxplayer.h \
  include/aoshoutplayer.h \
  include/aotextarea.h \
  include/aotimer.h \
  include/courtroom.h \
  include/datatypes.h \
  include/debug_functions.h \
  include/discord_rich_presence.h \
  include/encryption_functions.h \
  include/file_functions.h \
  include/hardware_functions.h \
  include/hex_functions.h \
  include/lobby.h \
  include/misc_functions.h \
  include/networkmanager.h

SOURCES += \
  src/aoabstractplayer.cpp \
  src/aoapplication.cpp \
  src/aobasshandle.cpp \
  src/aoblipplayer.cpp \
  src/aobutton.cpp \
  src/aocharbutton.cpp \
  src/aocharmovie.cpp \
  src/aoconfig.cpp \
  src/aoconfigpanel.cpp \
  src/aoemotebutton.cpp \
  src/aoevidencebutton.cpp \
  src/aoevidencedescription.cpp \
  src/aoevidencedisplay.cpp \
  src/aoexception.cpp \
  src/aoguiloader.cpp \
  src/aoimage.cpp \
  src/aolabel.cpp \
  src/aolineedit.cpp \
  src/aomovie.cpp \
  src/aomusicplayer.cpp \
  src/aonotearea.cpp \
  src/aonotepad.cpp \
  src/aonotepicker.cpp \
  src/aopacket.cpp \
  src/aopixmap.cpp \
  src/aoscene.cpp \
  src/aosfxplayer.cpp \
  src/aoshoutplayer.cpp \
  src/aotextarea.cpp \
  src/aotimer.cpp \
  src/audio_functions.cpp \
  src/charselect.cpp \
  src/courtroom.cpp \
  src/courtroom_widgets.cpp \
  src/debug_functions.cpp \
  src/discord_rich_presence.cpp \
  src/emotes.cpp \
  src/encryption_functions.cpp \
  src/evidence.cpp \
  src/file_functions.cpp \
  src/hardware_functions.cpp \
  src/hex_functions.cpp \
  src/lobby.cpp \
  src/main.cpp \
  src/misc_functions.cpp \
  src/networkmanager.cpp \
  src/packet_distribution.cpp \
  src/path_functions.cpp \
  src/text_file_functions.cpp

# 1. You need to get BASS and put the x86 bass DLL/headers in the project root folder
#    AND the compilation output folder. If you want a static link, you'll probably
#    need the .lib file too. MinGW-GCC is really finicky finding BASS, it seems.
# 2. You need to compile the Discord Rich Presence SDK separately and add the lib/headers
#    in the same way as BASS. Discord RPC uses CMake, which does not play nicely with
#    QMake, so this step must be manual.
unix:LIBS += -L$$PWD -lbass -ldiscord-rpc
win32:LIBS += -L$$PWD/3rd -lbass -ldiscord-rpc #"$$PWD/discord-rpc.dll"
android:LIBS += -L$$PWD\android\libs\armeabi-v7a\ -lbass

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
  res.qrc

DISTFILES +=

FORMS += \
  res/ui/config_panel.ui
