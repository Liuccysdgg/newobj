CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS LIB_QT LIB_HPSOCKET QT5

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

# DEFINES += LIB_QT
# DEFINES += QT5
DEFINES += LIB_HPSOCKET

DESTDIR = ../bin/

CONFIG(debug,debug|release){
    TARGET = newobj_x64_debug
}
CONFIG(release,debug|release){
    TARGET = newobj_x64_release
}

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../src/algorithm/stl.hpp \
    ../src/db/db.h \
    ../src/db/db_gen.hpp \
    ../src/db/db_sqlite3.h \
    ../src/log/log.h \
    ../src/log/log_define.h \
    ../src/log/log_no.h \
    ../src/log/log_slice.h \
    ../src/mgr/res_mgr.h \
    ../src/other/logic_run.h \
    ../src/other/node.h \
    ../src/other/or_core.hpp \
    ../src/other/or_perfrecord.h \
    ../src/other/or_program.h \
    ../src/other/or_task_handle.hpp \
    ../src/other/or_task_router.hpp \
    ../src/public/base.h \
    ../src/public/base.h \
    ../src/public/boolean.h \
    ../src/public/boolean.h \
    ../src/public/buffer.h \
    ../src/public/buffer.h \
    ../src/public/decimal.h \
    ../src/public/decimal.h \
    ../src/public/define.h \
    ../src/public/define.h \
    ../src/public/environment.h \
    ../src/public/environment.h \
    ../src/public/exception.h \
    ../src/public/exception.h \
    ../src/public/exception.h.save-failed \
    ../src/public/exception.h.save-failed \
    ../src/public/extra_data.h \
    ../src/public/extra_data.h \
    ../src/public/integer.hpp \
    ../src/public/integer.hpp \
    ../src/public/nobject.h \
    ../src/public/nobject.h \
    ../src/public/public.h \
    ../src/public/public.h \
    ../src/public/qwebengine.hpp \
    ../src/public/qwebengine.hpp \
    ../src/public/string.h \
    ../src/public/string.h \
    ../src/public/struct.h \
    ../src/public/struct.h \
    ../src/public/var.h \
    ../src/public/var.h \
    ../src/sdk/aliyun/oss.h \
    ../src/sdk/codec/base64.h \
    ../src/sdk/codec/gmult.h \
    ../src/sdk/codec/qaesencryption.h \
    ../src/sdk/codec/url.h \
    ../src/sdk/debug/log.h \
    ../src/sdk/ftp/ftplib.h \
    ../src/sdk/image/qrencode/bitstream.h \
    ../src/sdk/image/qrencode/config.h \
    ../src/sdk/image/qrencode/mask.h \
    ../src/sdk/image/qrencode/mmask.h \
    ../src/sdk/image/qrencode/mqrspec.h \
    ../src/sdk/image/qrencode/qrencode.h \
    ../src/sdk/image/qrencode/qrencode_inner.h \
    ../src/sdk/image/qrencode/qrinput.h \
    ../src/sdk/image/qrencode/qrspec.h \
    ../src/sdk/image/qrencode/rsecc.h \
    ../src/sdk/image/qrencode/split.h \
    ../src/sdk/image/recognition/cjy/cjy.h \
    ../src/sdk/libcurl/libcurl.hpp \
    ../src/sdk/lua/module.hpp \
    ../src/sdk/lua/vm.h \
    ../src/sdk/mem/MemoryPool.h \
    ../src/sdk/mem/MemoryPool.tcc \
    ../src/sdk/safe/define.h \
    ../src/sdk/text/htmlcxx/CharsetConverter.h \
    ../src/sdk/text/htmlcxx/Extensions.h \
    ../src/sdk/text/htmlcxx/Node.h \
    ../src/sdk/text/htmlcxx/ParserDom.h \
    ../src/sdk/text/htmlcxx/ParserSax.h \
    ../src/sdk/text/htmlcxx/ParserSax.tcc \
    ../src/sdk/text/htmlcxx/Uri.h \
    ../src/sdk/text/htmlcxx/ci_string.h \
    ../src/sdk/text/htmlcxx/css_lex.h \
    ../src/sdk/text/htmlcxx/css_syntax.h \
    ../src/sdk/text/htmlcxx/debug.h \
    ../src/sdk/text/htmlcxx/parser.h \
    ../src/sdk/text/htmlcxx/parser_pp.h \
    ../src/sdk/text/htmlcxx/tld.h \
    ../src/sdk/text/htmlcxx/tree.h \
    ../src/sdk/text/htmlcxx/utils.h \
    ../src/sdk/text/htmlcxx/wincstring.h \
    ../src/sdk/text/tinyxml/tinystr.h \
    ../src/sdk/text/tinyxml/tinyxml.h \
    ../src/sdk/warez/vmprotect/VMProtectDDK.h \
    ../src/sdk/warez/vmprotect/VMProtectSDK.h \
    ../src/sdk/warez/vmprotect/vmprotect.h \
    ../src/sdk/zip/miniz.h \
    ../src/sdk/zip/zip.h \
    ../src/template/tp_pageing.h \
    ../src/template/tp_pageing.h \
    ../src/template/tp_sqlstatement.h \
    ../src/template/tp_sqlstatement.h \
    ../src/template/tp_streamproc.hpp \
    ../src/template/tp_streamproc.hpp \
    ../src/template/tp_tcpserver.hpp \
    ../src/template/tp_tcpserver.hpp \
    ../src/template/tp_urlparam.h \
    ../src/template/tp_urlparam.h \
    ../src/util/browser.h \
    ../src/util/build.h \
    ../src/util/bytes.h \
    ../src/util/cache.h \
    ../src/util/codec.h \
    ../src/util/compress.h \
    ../src/util/config.h \
    ../src/util/debug.h \
    ../src/util/desktop.h \
    ../src/util/file.h \
    ../src/util/forstring.h \
    ../src/util/html.h \
    ../src/util/idx_maker.h \
    ../src/util/json.h \
    ../src/util/keyboard.h \
    ../src/util/lock.h \
    ../src/util/map.hpp \
    ../src/util/math.h \
    ../src/util/md5.h \
    ../src/util/mem.h \
    ../src/util/mouse.h \
    ../src/util/network.h \
    ../src/util/package.h \
    ../src/util/point.hpp \
    ../src/util/pool.hpp \
    ../src/util/print.h \
    ../src/util/process.h \
    ../src/util/queue.hpp \
    ../src/util/system.h \
    ../src/util/thread.h \
    ../src/util/time.h \
    ../src/util/timeout.h \
    ../src/util/util.h \
    ../src/util/util_lock.cpp.autosave \
    ../src/util/vector.hpp \
    ../src/util/verify.h \
    ../src/util/window.h \
    ../src/util/zip_test.h

SOURCES += \
    ../src/db/db_sqlite3.cpp \
    ../src/log/log_define.cpp \
    ../src/log/log_no.cpp \
    ../src/log/log_slice.cpp \
    ../src/mgr/res_mgr.cpp \
    ../src/other/logic_run.cpp \
    ../src/other/node.cpp \
    ../src/other/perfrecord.cpp \
    ../src/other/program.cpp \
    ../src/public/boolean.cpp \
    ../src/public/boolean.cpp \
    ../src/public/buffer.cpp \
    ../src/public/buffer.cpp \
    ../src/public/decimal.cpp \
    ../src/public/decimal.cpp \
    ../src/public/environment.cpp \
    ../src/public/environment.cpp \
    ../src/public/exception.cpp \
    ../src/public/exception.cpp \
    ../src/public/extra_data.cpp \
    ../src/public/extra_data.cpp \
    ../src/public/nobject.cpp \
    ../src/public/nobject.cpp \
    ../src/public/string.cpp \
    ../src/public/string.cpp \
    ../src/public/var.cpp \
    ../src/public/var.cpp \
    ../src/sdk/aliyun/oss.cpp \
    ../src/sdk/codec/base64.cpp \
    ../src/sdk/codec/gmult.c \
    ../src/sdk/codec/qaesencryption.cpp \
    ../src/sdk/codec/url.cpp \
    ../src/sdk/debug/log.cpp \
    ../src/sdk/ftp/ftplib.cpp \
    ../src/sdk/image/qrencode/bitstream.c \
    ../src/sdk/image/qrencode/mask.c \
    ../src/sdk/image/qrencode/mmask.c \
    ../src/sdk/image/qrencode/mqrspec.c \
    ../src/sdk/image/qrencode/qrencode.c \
    ../src/sdk/image/qrencode/qrinput.c \
    ../src/sdk/image/qrencode/qrspec.c \
    ../src/sdk/image/qrencode/rsecc.c \
    ../src/sdk/image/qrencode/split.c \
    ../src/sdk/image/recognition/cjy/cjy.cpp \
    ../src/sdk/libcurl/libcurl.cpp \
    ../src/sdk/lua/vm.cpp \
    ../src/sdk/text/htmlcxx/CharsetConverter.cc \
    ../src/sdk/text/htmlcxx/Extensions.cc \
    ../src/sdk/text/htmlcxx/Node.cc \
    ../src/sdk/text/htmlcxx/ParserDom.cc \
    ../src/sdk/text/htmlcxx/ParserSax.cc \
    ../src/sdk/text/htmlcxx/Uri.cc \
    ../src/sdk/text/htmlcxx/css_lex.c \
    ../src/sdk/text/htmlcxx/css_syntax.c \
    ../src/sdk/text/htmlcxx/parser.c \
    ../src/sdk/text/htmlcxx/parser_pp.cc \
    ../src/sdk/text/htmlcxx/uriTests.cc \
    ../src/sdk/text/htmlcxx/utils.cc \
    ../src/sdk/text/tinyxml/tinystr.cpp \
    ../src/sdk/text/tinyxml/tinyxml.cpp \
    ../src/sdk/text/tinyxml/tinyxmlerror.cpp \
    ../src/sdk/text/tinyxml/tinyxmlparser.cpp \
    ../src/sdk/zip/zip.cpp \
    ../src/template/tp_pageing.cpp \
    ../src/template/tp_pageing.cpp \
    ../src/template/tp_sqlstatement.cpp \
    ../src/template/tp_sqlstatement.cpp \
    ../src/template/tp_urlparam.cpp \
    ../src/template/tp_urlparam.cpp \
    ../src/util/browser.cpp \
    ../src/util/build.cpp \
    ../src/util/bytes.cpp \
    ../src/util/cache.cpp \
    ../src/util/codec.cpp \
    ../src/util/codec_des.cpp \
    ../src/util/compress.cpp \
    ../src/util/config.cpp \
    ../src/util/debug.cpp \
    ../src/util/desktop.cpp \
    ../src/util/file.cpp \
    ../src/util/forstring.cpp \
    ../src/util/html.cpp \
    ../src/util/idx_maker.cpp \
    ../src/util/json.cpp \
    ../src/util/keyboard.cpp \
    ../src/util/lock.cpp \
    ../src/util/math.cpp \
    ../src/util/md5.cpp \
    ../src/util/mem.cpp \
    ../src/util/mouse.cpp \
    ../src/util/network.cpp \
    ../src/util/obj.cpp \
    ../src/util/package.cpp \
    ../src/util/print.cpp \
    ../src/util/process.cpp \
    ../src/util/system.cpp \
    ../src/util/thread.cpp \
    ../src/util/time.cpp \
    ../src/util/timeout.cpp \
    ../src/util/verify.cpp \
    ../src/util/window.cpp \
    ../src/util/zip_test.cpp

DISTFILES += \
    ../src/other/or_core_config_json_template.json \
    ../src/sdk/mysql/mysql-connector-c-6.1.11-src.tar.gz
