CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

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

DEFINES += LIB_QT
DEFINES += QT5

CONFIG(debug,debug|release){
    TARGET = module_qt_x64_debug_qt5
}
CONFIG(release,debug|release){
    TARGET = module_qt_x64_release_qt5
}



# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../module/qt/ui/button.h \
    ../module/qt/ui/checkbox.h \
    ../module/qt/ui/highlighter.h \
    ../module/qt/ui/image.h \
    ../module/qt/ui/label.h \
    ../module/qt/ui/lineedit.h \
    ../module/qt/ui/topbar.hpp \
    ../module/qt/ui/widget.hpp \
    ../module/qt/util/file.h \
    ../module/qt/util/system.h \
    ../module/qt/util/ui.h

SOURCES += \
    ../module/qt/ui/button.cpp \
    ../module/qt/ui/checkbox.cpp \
    ../module/qt/ui/highlighter.cpp \
    ../module/qt/ui/image.cpp \
    ../module/qt/ui/label.cpp \
    ../module/qt/ui/lineedit.cpp \
    ../module/qt/util/file.cpp \
    ../module/qt/util/system.cpp \
    ../module/qt/util/ui.cpp
