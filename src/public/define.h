#pragma once
#ifdef QT_CORE_LIB
#include "QtCore/qstring.h"
#endif
#include "typedef.h"
#include "macro.h"
#include "../util/mem.h"

/***************************************************[����]***************************************************/
// HPSocket ����
#define LIB_HPSOCKET
// HPSocket ��̬����
#define HPSOCKET_STATIC_LIB
// HPSocket ֧��SSL
#define LIB_HPSOCKET_SSL

#ifdef _WIN32
#define LIB_QT
#else
// Linuxƽ̨������CMakeLists.txt,Ĭ�Ϲر�QT����
#endif
// QT
#ifdef LIB_QT
#ifdef MSVC_2010
// QT4
#define QT4
#else
// QT5
#define QT5
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



