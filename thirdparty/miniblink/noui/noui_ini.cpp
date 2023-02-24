#ifndef MSVC_2010
#include "noui_ini.h"
#include "miniblink/miniblink.h"
#include "util/config.h"
#include "util/codec.h"
#if LIB_QT == 1
newobj::noui::ini::ini(newobj::browser::miniblink* miniblink)
	:noui::core(miniblink)
{
	// write
	BIND_FUNCTION("noui_ini_write", {
		return JS_BOOL(((newobj::noui::ini*)param)->write(
			ARG_STRING(0),
			ARG_STRING(1),
			ARG_STRING(2),
			ARG_STRING(3)
		));
    },4);
	// read
	BIND_FUNCTION("noui_ini_read", {
		nstring result = ((newobj::noui::ini*)param)->read(ARG_STRING(0),ARG_STRING(1),ARG_STRING(2));
		return JS_STRING(result.c_str());
	},3);
}

newobj::noui::ini::~ini()
{
}
/*–¥≈‰÷√*/
bool newobj::noui::ini::write(const nstring& path, const nstring& node, const nstring& name, const nstring& value)
{
	return newobj::config::ini::write(codec::to_gbk(path),node,name,value);
}
/*∂¡≈‰÷√*/
nstring newobj::noui::ini::read(const nstring& path, const nstring& node, const nstring& name)
{
	return newobj::config::ini::read(codec::to_gbk(path), node, name);
}
#endif
#endif
