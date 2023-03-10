#pragma once
#ifdef QT_CORE_LIB
#include "QtCore/qstring.h"
#endif
#include "typedef.h"
#include "macro.h"
#include "../util/mem.h"

/***************************************************[功能]***************************************************/
// HPSocket 开关
#define LIB_HPSOCKET
// HPSocket 静态编译
#define HPSOCKET_STATIC_LIB
// HPSocket 支持SSL
#define LIB_HPSOCKET_SSL
// mimalloc 内存池
#define LIB_MIMALLOC 1

// Log4cplus 开关
#ifdef _WIN32
#define LIB_LOG4CPLUS 0
#else
#define LIB_LOG4CPLUS 1
#endif

#ifdef _WIN32
#ifdef MSVC_2010
#define LIB_QT 0
#else
#define LIB_QT 1
#endif
#else
#define LIB_QT 0
// Linux平台请设置CMakeLists.txt,默认关闭QT链接
#endif
// QT
#if LIB_QT == 1
#ifdef MSVC_2010
// QT4
#define QT4 1
#else
// QT5
#define QT5 1
#endif
/****************************[ QT ]************************************/
#if LIB_QT == 1
#define QSENDER(TO_OBJ) qobject_cast<TO_OBJ>(sender())
#define QT_WAIT(TIMEOUT_MSEC,LOGIC)                                     \
            {                                                                                           \
                QTime time;                                                                     \
                    time.start();                                                                   \
                while (time.elapsed() < (int)TIMEOUT_MSEC && (LOGIC))\
                    QCoreApplication::processEvents();                              \
            }
#endif
#if LIB_QT == 1
#ifndef QT4
#define QTSTR(TEXT) QStringLiteral(TEXT)
#else
#define QTSTR(TEXT) QObject::tr(TEXT)
#endif
#endif
#endif


namespace newobj {

}





#ifdef _WIN64
	
#else
	
#endif

using namespace newobj;




//#define MSVC_VER _MSC_VER 
//#define MSVC_2010 1600
//#define MSVC_2012 1700 
//#define MSVC_2013 1800 
//#define MSVC_2015 1900 
//#define MSVC_2017 2000
//#define MSVC_2019 2100



