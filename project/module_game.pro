CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS LIB_QT QT5

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../src
INCLUDEPATH += ../include
INCLUDEPATH += ../module
INCLUDEPATH += ../thirdparty
INCLUDEPATH += ../thirdparty/hpsocket
INCLUDEPATH += ../include/qt/5.9.9_static


DESTDIR = ../bin/

CONFIG(debug,debug|release){
    TARGET = module_game_x64_debug
}
CONFIG(release,debug|release){
    TARGET = module_game_x64_release
}



# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../module/network/CMakeLists.txt \
    ../module/network/network.vcxproj \
    ../module/network/network.vcxproj.filters

HEADERS += \
    ../module/game/cq/define.h \
    ../module/game/cq/enade.hpp \
    ../module/game/cq/engine_account.h \
    ../module/game/cq/engine_clear_data.h \
    ../module/game/cq/engine_clear_username.h \
    ../module/game/cq/engine_clear_var.h \
    ../module/game/cq/engine_hequ.h \
    ../module/game/cq/engine_init.h \
    ../module/game/cq/engine_loadoffline.h \
    ../module/game/cq/engine_loadoffline_m2.h \
    ../module/game/cq/engine_notice.h \
    ../module/game/cq/engine_role.h \
    ../module/game/cq/engine_start.h \
    ../module/game/cq/engine_stop.h \
    ../module/game/cq/engine_util.h \
    ../module/game/cq/login.h \
    ../module/game/cq/logingate_client.h \
    ../module/game/cq/loginlist_client.h \
    ../module/game/cq/engine_reloadnpc.h

SOURCES += \
    ../module/game/cq/engine_account.cpp \
    ../module/game/cq/engine_clear_data.cpp \
    ../module/game/cq/engine_clear_username.cpp \
    ../module/game/cq/engine_clear_var.cpp \
    ../module/game/cq/engine_hequ.cpp \
    ../module/game/cq/engine_init.cpp \
    ../module/game/cq/engine_loadoffline.cpp \
    ../module/game/cq/engine_loadoffline_m2.cpp \
    ../module/game/cq/engine_notice.cpp \
    ../module/game/cq/engine_role.cpp \
    ../module/game/cq/engine_start.cpp \
    ../module/game/cq/engine_stop.cpp \
    ../module/game/cq/engine_util.cpp \
    ../module/game/cq/login.cpp \
    ../module/game/cq/logingate_client.cpp \
    ../module/game/cq/loginlist_client.cpp \
    ../module/game/cq/engine_reloadnpc.cpp


