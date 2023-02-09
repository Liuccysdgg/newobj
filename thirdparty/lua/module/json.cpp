#include "json.h"
#include "define.h"
module_config<newobj::lua::module::json>* m_config = nullptr;
newobj::lua::module::json::json()
{
}


newobj::lua::module::json::~json()
{
}

bool newobj::lua::module::json::parse_file(const std::string& filepath)
{
	return m_json.parse_file(filepath);
}

newobj::lua::module::json* newobj::lua::module::json::at(int index)
{
	auto result_ptr = m_config->create();
	try
	{
		auto result = m_json[index];
		result_ptr->m_json = result;
		return result_ptr;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return result_ptr;
}

int newobj::lua::module::json::at_int(int index)
{
	
	return m_json[index].to<int>();
}

std::string newobj::lua::module::json::at_string(int index)
{
	return m_json[index].to <std::string> ();
}

double newobj::lua::module::json::at_double(int index)
{
	return m_json[index].to<double>();
}

bool newobj::lua::module::json::at_bool(int index)
{
	return m_json[index].to<bool>();
}

std::string newobj::lua::module::json::to_string()
{
	return m_json.to_string();
}
std::string newobj::lua::module::json::get_string(const std::string& name)
{
	return m_json[name].to<nstring>();
}
int newobj::lua::module::json::get_int(const std::string& name)
{
	return m_json[name].to<int>();
}
double newobj::lua::module::json::get_double(const std::string& name)
{
	return m_json[name].to<double>();
}
bool newobj::lua::module::json::get_bool(const std::string& name)
{
	return m_json[name].to<bool>();
}
newobj::lua::module::json* newobj::lua::module::json::get_obj(const std::string& name)
{
	auto result = m_config->create();
	result->m_json = m_json[name];
	return result;
}
void newobj::lua::module::json::set_path(const std::string path)
{
	m_json.path(path);
}
bool newobj::lua::module::json::parse(const std::string& value)
{
	return m_json.parse(value);
}
bool newobj::lua::module::json::save(const std::string& save)
{
	return m_json.save(save);
}
int newobj::lua::module::json::size()
{
	return m_json.size();
}
void newobj::lua::module::json::resize(int size)
{
	m_json.resize(size);
}
bool newobj::lua::module::json::exist(const std::string& name)
{
	return m_json.exist(name);
}
void newobj::lua::module::json::erase_by_name(const std::string& name)
{
	m_json.erase(name);
}
void newobj::lua::module::json::erase_by_index(int index)
{
	m_json.erase(index);
}
void newobj::lua::module::json::push_int(int value)
{
	return m_json.push_back(value);
}
void newobj::lua::module::json::push_string(const std::string& value)
{
	return m_json.push_back(value);
}
void newobj::lua::module::json::push_double(double value)
{
	return m_json.push_back(value);
}
void newobj::lua::module::json::push_bool(bool value)
{
	return m_json.push_back(value);
}
void newobj::lua::module::json::set_int(const std::string& name, int value)
{
	m_json[name] = value;
}
void newobj::lua::module::json::set_string(const std::string& name, const std::string& value)
{
	m_json[name] = value;
}
void newobj::lua::module::json::set_double(const std::string& name, double value)
{
	m_json[name] = value;
}
void newobj::lua::module::json::set_bool(const std::string& name, bool value)
{
	m_json[name] = value;
}
void newobj::lua::module::regist_json(void* config, kaguya::State* L)
{
	m_config = (module_config<newobj::lua::module::json>*)config;
	auto table = kaguya::UserdataMetatable<newobj::lua::module::json>();
	table.setConstructors<newobj::lua::module::json()>();
	table.addFunction("parse_file", &newobj::lua::module::json::parse_file);
	table.addFunction("at", &newobj::lua::module::json::at);
	table.addFunction("to_string", &newobj::lua::module::json::to_string);
	table.addFunction("get_string", &newobj::lua::module::json::get_string);
	table.addFunction("get_bool", &newobj::lua::module::json::get_bool);
	table.addFunction("get_double", &newobj::lua::module::json::get_double);
	table.addFunction("get_obj", &newobj::lua::module::json::get_obj);
	table.addFunction("get_int", &newobj::lua::module::json::get_int);

	table.addFunction("set_path", &newobj::lua::module::json::set_path);
	table.addFunction("parse", &newobj::lua::module::json::parse);
	table.addFunction("save", &newobj::lua::module::json::save);
	table.addFunction("size", &newobj::lua::module::json::size);
	table.addFunction("resize", &newobj::lua::module::json::resize);
	table.addFunction("exist", &newobj::lua::module::json::exist);
	table.addFunction("erase_by_name", &newobj::lua::module::json::erase_by_name);
	table.addFunction("erase_by_index", &newobj::lua::module::json::erase_by_index);
	table.addFunction("push_int", &newobj::lua::module::json::push_int);
	table.addFunction("push_string", &newobj::lua::module::json::push_string);
	table.addFunction("push_double", &newobj::lua::module::json::push_double);
	table.addFunction("push_bool", &newobj::lua::module::json::push_bool);

	table.addFunction("at_int", &newobj::lua::module::json::at_int);
	table.addFunction("at_string", &newobj::lua::module::json::at_string);
	table.addFunction("at_double", &newobj::lua::module::json::at_double);
	table.addFunction("at_bool", &newobj::lua::module::json::at_bool);

	table.addFunction("set_int", &newobj::lua::module::json::set_int);
	table.addFunction("set_string", &newobj::lua::module::json::set_string);
	table.addFunction("set_double", &newobj::lua::module::json::set_double);
	table.addFunction("set_bool", &newobj::lua::module::json::set_bool);

	(*L)["JSON"].setClass(table);
}

