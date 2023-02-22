CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS NO_FSEEKO

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../src
INCLUDEPATH += ../include
INCLUDEPATH += ../module
INCLUDEPATH += ../thirdparty
INCLUDEPATH += ../thirdparty/lua


DESTDIR = ../bin/

CONFIG(debug,debug|release){
    TARGET = thirdparty_mysql_x64_debug
}
CONFIG(release,debug|release){
    TARGET = thirdparty_mysql_x64_release
}



# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../thirdparty/mysql/mysql.h

SOURCES += \
    ../thirdparty/mysql/mysql.cpp




