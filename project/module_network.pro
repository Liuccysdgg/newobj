CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS LIB_HPSOCKET

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../src
INCLUDEPATH += ../include
INCLUDEPATH += ../module
INCLUDEPATH += ../thirdparty
INCLUDEPATH += ../thirdparty/hpsocket



DESTDIR = ../bin/

CONFIG(debug,debug|release){
    TARGET = module_network_x64_debug
}
CONFIG(release,debug|release){
    TARGET = module_network_x64_release
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
    ../module/network/ftp/client.h \
    ../module/network/http/http_center.h \
    ../module/network/http/http_client.h \
    ../module/network/http/http_client_cache.h \
    ../module/network/http/http_controller.h \
    ../module/network/http/http_cookie.h \
    ../module/network/http/http_define.h \
    ../module/network/http/http_header.h \
    ../module/network/http/http_interceptor.h \
    ../module/network/http/http_interface.h \
    ../module/network/http/http_parser.h \
    ../module/network/http/http_reqpack.h \
    ../module/network/http/http_request.h \
    ../module/network/http/http_response.h \
    ../module/network/http/http_router.h \
    ../module/network/http/http_server.h \
    ../module/network/http/http_server_lst.h \
    ../module/network/http/http_session.h \
    ../module/network/http/http_util.h \
    ../module/network/http/make_form.h \
    ../module/network/iclient.h \
    ../module/network/ictl.h \
    ../module/network/inet.h \
    ../module/network/iserver.h \
    ../module/network/other/bigfile_client.h \
    ../module/network/other/bigfile_common.h \
    ../module/network/other/bigfile_server.h \
    ../module/network/other/can.h \
    ../module/network/tcp/tcp_client.h \
    ../module/network/tcp/tcp_client_lst.h \
    ../module/network/tcp/tcp_forward.h \
    ../module/network/tcp/tcp_server.h \
    ../module/network/tcp/tcp_server_lst.h \
    ../module/network/udp/udp_node.h \
    ../module/network/udp/udp_node_lst.h

SOURCES += \
    ../module/network/ftp/client.cpp \
    ../module/network/http/http_center.cpp \
    ../module/network/http/http_client.cpp \
    ../module/network/http/http_client_cache.cpp \
    ../module/network/http/http_controller.cpp \
    ../module/network/http/http_cookie.cpp \
    ../module/network/http/http_header.cpp \
    ../module/network/http/http_interceptor.cpp \
    ../module/network/http/http_interface.cpp \
    ../module/network/http/http_parser.cpp \
    ../module/network/http/http_reqpack.cpp \
    ../module/network/http/http_request.cpp \
    ../module/network/http/http_response.cpp \
    ../module/network/http/http_router.cpp \
    ../module/network/http/http_server.cpp \
    ../module/network/http/http_server_lst.cpp \
    ../module/network/http/http_session.cpp \
    ../module/network/http/http_util.cpp \
    ../module/network/http/make_form.cpp \
    ../module/network/iclient.cpp \
    ../module/network/inet.cpp \
    ../module/network/iserver.cpp \
    ../module/network/other/bigfile_client.cpp \
    ../module/network/other/bigfile_server.cpp \
    ../module/network/other/can.cpp \
    ../module/network/tcp/tcp_client.cpp \
    ../module/network/tcp/tcp_client_lst.cpp \
    ../module/network/tcp/tcp_forward.cpp \
    ../module/network/tcp/tcp_server.cpp \
    ../module/network/tcp/tcp_server_lst.cpp \
    ../module/network/udp/udp_node.cpp \
    ../module/network/udp/udp_node_lst.cpp
