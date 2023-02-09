#include "vector_int.h"
#include "util/json.h"
#include "define.h"
#include <map>
#include "algorithm/stl.hpp"
void newobj::lua::module::vector_int::push_back(int value)
{
	m_value.push_back(value);
}

int newobj::lua::module::vector_int::at(int index)
{
	return m_value.at(index);
}

void newobj::lua::module::vector_int::clear()
{
	m_value.clear();
}

void newobj::lua::module::vector_int::sort(bool ps)
{
	stl::sort(m_value,ps);
}

uint32 newobj::lua::module::vector_int::size()
{
	return m_value.size();
}

module_config<newobj::lua::module::vector_int> *m_config = nullptr;

void newobj::lua::module::regist_vector(void* config, kaguya::State* L)
{
	m_config = (module_config<newobj::lua::module::vector_int>*)config;


	auto table_file = kaguya::UserdataMetatable<newobj::lua::module::vector_int>();
	table_file.setConstructors<newobj::lua::module::vector_int()>();
	table_file.addFunction("push_back", &newobj::lua::module::vector_int::push_back);
	table_file.addFunction("at", &newobj::lua::module::vector_int::at);
	table_file.addFunction("clear", &newobj::lua::module::vector_int::clear);
	table_file.addFunction("sort", &newobj::lua::module::vector_int::sort);
	table_file.addFunction("size", &newobj::lua::module::vector_int::size);
	(*L)["VECTOR_INT"].setClass(table_file);
}

newobj::lua::module::vector_int::vector_int()
{
}

newobj::lua::module::vector_int::~vector_int()
{
}

