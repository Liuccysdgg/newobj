#include "vector_string.h"
#include "util/json.h"
#include "define.h"
#include <map>
#include "algorithm/stl.hpp"
void newobj::lua::module::vector_string::push_back(const std::string& value)
{
	m_value.push_back(value);
}

std::string newobj::lua::module::vector_string::at(int index)
{
	return m_value.at(index);
}

void newobj::lua::module::vector_string::clear()
{
	m_value.clear();
}
uint32 newobj::lua::module::vector_string::size()
{
	return m_value.size();
}

module_config<newobj::lua::module::vector_string> *m_config = nullptr;

void newobj::lua::module::regist_vector_string(void* config, kaguya::State* L)
{
	m_config = (module_config<newobj::lua::module::vector_string>*)config;


	auto table_file = kaguya::UserdataMetatable<newobj::lua::module::vector_string>();
	table_file.setConstructors<newobj::lua::module::vector_string()>();
	table_file.addFunction("push_back", &newobj::lua::module::vector_string::push_back);
	table_file.addFunction("at", &newobj::lua::module::vector_string::at);
	table_file.addFunction("clear", &newobj::lua::module::vector_string::clear);
	table_file.addFunction("size", &newobj::lua::module::vector_string::size);
	(*L)["VECTOR_STRING"].setClass(table_file);
}

newobj::lua::module::vector_string::vector_string()
{
}

newobj::lua::module::vector_string::~vector_string()
{
}

