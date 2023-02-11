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
    TARGET = thirdparty_lua_x64_debug
}
CONFIG(release,debug|release){
    TARGET = thirdparty_lua_x64_release
}



# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../thirdparty/lua/core/lapi.h \
    ../thirdparty/lua/core/lauxlib.h \
    ../thirdparty/lua/core/lcode.h \
    ../thirdparty/lua/core/lctype.h \
    ../thirdparty/lua/core/ldebug.h \
    ../thirdparty/lua/core/ldo.h \
    ../thirdparty/lua/core/lfunc.h \
    ../thirdparty/lua/core/lgc.h \
    ../thirdparty/lua/core/ljumptab.h \
    ../thirdparty/lua/core/llex.h \
    ../thirdparty/lua/core/llimits.h \
    ../thirdparty/lua/core/lmem.h \
    ../thirdparty/lua/core/lobject.h \
    ../thirdparty/lua/core/lopcodes.h \
    ../thirdparty/lua/core/lopnames.h \
    ../thirdparty/lua/core/lparser.h \
    ../thirdparty/lua/core/lprefix.h \
    ../thirdparty/lua/core/lstate.h \
    ../thirdparty/lua/core/lstring.h \
    ../thirdparty/lua/core/ltable.h \
    ../thirdparty/lua/core/ltm.h \
    ../thirdparty/lua/core/lua.h \
    ../thirdparty/lua/core/lua.hpp \
    ../thirdparty/lua/core/luaconf.h \
    ../thirdparty/lua/core/lualib.h \
    ../thirdparty/lua/core/lundump.h \
    ../thirdparty/lua/core/lvm.h \
    ../thirdparty/lua/core/lzio.h \
    ../thirdparty/lua/kaguya/another_binding_api.hpp \
    ../thirdparty/lua/kaguya/compatibility.hpp \
    ../thirdparty/lua/kaguya/config.hpp \
    ../thirdparty/lua/kaguya/detail/lua_function_def.hpp \
    ../thirdparty/lua/kaguya/detail/lua_ref_impl.hpp \
    ../thirdparty/lua/kaguya/detail/lua_table_def.hpp \
    ../thirdparty/lua/kaguya/detail/lua_variant_def.hpp \
    ../thirdparty/lua/kaguya/error_handler.hpp \
    ../thirdparty/lua/kaguya/exception.hpp \
    ../thirdparty/lua/kaguya/function_tuple_def.hpp \
    ../thirdparty/lua/kaguya/kaguya.hpp \
    ../thirdparty/lua/kaguya/lua_ref.hpp \
    ../thirdparty/lua/kaguya/lua_ref_function.hpp \
    ../thirdparty/lua/kaguya/lua_ref_table.hpp \
    ../thirdparty/lua/kaguya/metatable.hpp \
    ../thirdparty/lua/kaguya/native_function.hpp \
    ../thirdparty/lua/kaguya/native_function_cxx03.hpp \
    ../thirdparty/lua/kaguya/native_function_cxx11.hpp \
    ../thirdparty/lua/kaguya/object.hpp \
    ../thirdparty/lua/kaguya/optional.hpp \
    ../thirdparty/lua/kaguya/preprocess.hpp \
    ../thirdparty/lua/kaguya/preprocess_repeate.hpp \
    ../thirdparty/lua/kaguya/push_any.hpp \
    ../thirdparty/lua/kaguya/push_tuple.hpp \
    ../thirdparty/lua/kaguya/ref_tuple.hpp \
    ../thirdparty/lua/kaguya/state.hpp \
    ../thirdparty/lua/kaguya/traits.hpp \
    ../thirdparty/lua/kaguya/type.hpp \
    ../thirdparty/lua/kaguya/utility.hpp \
    ../thirdparty/lua/kaguya/utility_cxx03.hpp \
    ../thirdparty/lua/kaguya/utility_cxx11.hpp \
    ../thirdparty/lua/lua.h \
    ../thirdparty/lua/module/algorithm.h \
    ../thirdparty/lua/module/define.h \
    ../thirdparty/lua/module/file.h \
    ../thirdparty/lua/module/forstring.h \
    ../thirdparty/lua/module/ini.h \
    ../thirdparty/lua/module/json.h \
    ../thirdparty/lua/module/system.h \
    ../thirdparty/lua/module/time.h \
    ../thirdparty/lua/module/vector_int.h \
    ../thirdparty/lua/module/vector_string.h \
    ../thirdparty/lua/module/verify.h

SOURCES += \
    ../thirdparty/lua/core/lapi.c \
    ../thirdparty/lua/core/lauxlib.c \
    ../thirdparty/lua/core/lbaselib.c \
    ../thirdparty/lua/core/lcode.c \
    ../thirdparty/lua/core/lcorolib.c \
    ../thirdparty/lua/core/lctype.c \
    ../thirdparty/lua/core/ldblib.c \
    ../thirdparty/lua/core/ldebug.c \
    ../thirdparty/lua/core/ldo.c \
    ../thirdparty/lua/core/ldump.c \
    ../thirdparty/lua/core/lfunc.c \
    ../thirdparty/lua/core/lgc.c \
    ../thirdparty/lua/core/linit.c \
    ../thirdparty/lua/core/liolib.c \
    ../thirdparty/lua/core/llex.c \
    ../thirdparty/lua/core/lmathlib.c \
    ../thirdparty/lua/core/lmem.c \
    ../thirdparty/lua/core/loadlib.c \
    ../thirdparty/lua/core/lobject.c \
    ../thirdparty/lua/core/lopcodes.c \
    ../thirdparty/lua/core/loslib.c \
    ../thirdparty/lua/core/lparser.c \
    ../thirdparty/lua/core/lstate.c \
    ../thirdparty/lua/core/lstring.c \
    ../thirdparty/lua/core/lstrlib.c \
    ../thirdparty/lua/core/ltable.c \
    ../thirdparty/lua/core/ltablib.c \
    ../thirdparty/lua/core/ltm.c \
    ../thirdparty/lua/core/lua.c \
    ../thirdparty/lua/core/luac.c \
    ../thirdparty/lua/core/lundump.c \
    ../thirdparty/lua/core/lutf8lib.c \
    ../thirdparty/lua/core/lvm.c \
    ../thirdparty/lua/core/lzio.c \
    ../thirdparty/lua/lua.cpp \
    ../thirdparty/lua/module/algorithm.cpp \
    ../thirdparty/lua/module/file.cpp \
    ../thirdparty/lua/module/forstring.cpp \
    ../thirdparty/lua/module/ini.cpp \
    ../thirdparty/lua/module/json.cpp \
    ../thirdparty/lua/module/system.cpp \
    ../thirdparty/lua/module/time.cpp \
    ../thirdparty/lua/module/vector_int.cpp \
    ../thirdparty/lua/module/vector_string.cpp \
    ../thirdparty/lua/module/verify.cpp


