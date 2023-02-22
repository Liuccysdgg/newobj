#include "util/config.h"
#include "define.h"
#include <map>
#include "algorithm.h"
#include "vector_int.h"
newobj::lua::module::algorithm::algorithm()
{
}

newobj::lua::module::algorithm::~algorithm()
{
}


void newobj::lua::module::regist_algorithm(void* config, kaguya::State* L)
{
	//auto table = kaguya::UserdataMetatable<class newobj::lua::module::algorithm>();
	//table.addStaticFunction("sort_vector", [](const newobj::lua::module::vector_int* vector ,const std::string& node,const std::string& name)->std::string {
	//	return newobj::config::ini::read(path,node,name);
	//});
	//table.addStaticFunction("write", [](const std::string& path, const std::string& node, const std::string& name,const std::string& value)->bool {
	//	return newobj::config::ini::write(path, node, name,value);
	//});
	//(*L)["INI"].setClass(table);
}

