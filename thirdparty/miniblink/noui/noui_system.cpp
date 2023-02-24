#ifndef MSVC_2010
#include "noui_system.h"
#include "miniblink/miniblink.h"
#include "util/config.h"
#include "util/codec.h"
#if LIB_QT == 1
newobj::noui::system::system(newobj::browser::miniblink* miniblink)
	:noui::core(miniblink)
{
	//// write
	//BIND_FUNCTION("noui_ini_write", {
	//	return JS_BOOL(((newobj::noui::ini*)param)->write(
	//		ARG_STRING(0),
	//		ARG_STRING(1),
	//		ARG_STRING(2),
	//		ARG_STRING(3)
	//	));
 //   },4);
	//// read
	//BIND_FUNCTION("noui_ini_read", {
	//	nstring result = ((newobj::noui::ini*)param)->read(ARG_STRING(0),ARG_STRING(1),ARG_STRING(2));
	//	return JS_STRING(result.c_str());
	//},3);
}

newobj::noui::system::~system()
{
}
uint32 newobj::noui::system::device_size_width()
{
	return uint32();
}
uint32 newobj::noui::system::device_size_height()
{
	return uint32();
}
uint32 newobj::noui::system::work_size_width()
{
	return uint32();
}
uint32 newobj::noui::system::work_size_height()
{
	return uint32();
}
#endif
#endif
