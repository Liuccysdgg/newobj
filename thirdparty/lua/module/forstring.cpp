#include "util/config.h"
#include "define.h"
#include <map>
#include "forstring.h"
newobj::lua::module::forstring::forstring()
{
}

newobj::lua::module::forstring::~forstring()
{
}


void newobj::lua::module::regist_forstring(void* config, kaguya::State* L)
{
	auto table = kaguya::UserdataMetatable<class newobj::lua::module::forstring>();
	
	table.addStaticFunction("replace2", [](const std::string& value,const std::string& findvalue, const std::string& replacevalue)->std::string {
		return ((nstring)value).replace(findvalue, replacevalue);
	});
	table.addStaticFunction("left", [](const std::string& value, uint32 length)->std::string {
		return nstring(nstring(value).left(length)).c_str();
	});
	table.addStaticFunction("right", [](const std::string& value, uint32 length)->std::string {
		return nstring(nstring(value).right(length)).c_str();
	});
	table.addStaticFunction("replace", [](const std::string& value, uint32 start,uint32 length,const std::string& replace_value)->std::string {
		return ((nstring)value).replace(start,length,replace_value);
	});
	table.addStaticFunction("trim_end", [](const std::string& value, const std::string& c)->std::string {
		if (c.length() == 0)
			return value;
		return nstring(((nstring)value).trim_end(c[0])).c_str();
	});
	table.addStaticFunction("trim_begin", [](const std::string& value, const std::string& c)->std::string {
		if (c.length() == 0)
			return value;
		return ((nstring)value).trim_begin(c[0]);
	});
	table.addStaticFunction("find", [](const std::string& value, const std::string& c)->int {
		return ((nstring)value).find(c);
	});
	(*L)["FORSTRING"].setClass(table);
}

