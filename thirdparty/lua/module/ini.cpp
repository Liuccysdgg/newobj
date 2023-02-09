#include "util/config.h"
#include "define.h"
#include <map>
#include "ini.h"
newobj::lua::module::ini::ini()
{
}

newobj::lua::module::ini::~ini()
{
}


void newobj::lua::module::regist_ini(void* config, kaguya::State* L)
{
	auto table = kaguya::UserdataMetatable<class newobj::lua::module::ini>();
	table.addStaticFunction("read", [](const std::string& path,const std::string& node,const std::string& name)->std::string {
		return newobj::config::ini::read(path,node,name);
	});
	table.addStaticFunction("write", [](const std::string& path, const std::string& node, const std::string& name,const std::string& value)->bool {
		return newobj::config::ini::write(path, node, name,value);
	});
	(*L)["INI"].setClass(table);
}

