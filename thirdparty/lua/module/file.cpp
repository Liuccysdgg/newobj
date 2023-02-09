#include "file.h"
#include "util/json.h"
#include "define.h"
#include <map>


module_config<newobj::lua::module::file> *m_config = nullptr;
newobj::lua::module::file::file()
{
}

newobj::lua::module::file::~file()
{
}

bool newobj::lua::module::file::open(const std::string& filepath)
{
	return m_file.open(filepath);
}

bool newobj::lua::module::file::write(const std::string& value)
{
	return m_file.write(value);
}

bool newobj::lua::module::file::jump(uint64 idx)
{
	return m_file.jump(idx);
}

bool newobj::lua::module::file::close()
{
	return m_file.close();
}

bool newobj::lua::module::file::flush()
{
	return m_file.flush();
}

bool newobj::lua::module::file::clear()
{
	return m_file.clear();
}

std::string newobj::lua::module::file::read(uint32 idx, uint32 len)
{
	char* mem = new char[len+1];
	memset(mem, 0, len + 1);
	std::string result;
	if (m_file.read(idx, len, mem) == false)
	{
		delete[] mem;
		return "";
	}
	result = mem;
	return result;
}

std::string newobj::lua::module::file::filepath()
{
	return m_file.filepath();
}

std::string newobj::lua::module::file::read_all(const std::string& path)
{
	return newobj::file::read(path).to_string();
}

uint64 newobj::lua::module::file::size(const std::string& path)
{
	uint64 size = 0;
	if (newobj::file::size(path, &size) == false)
		return 0;
	return size;
}

bool newobj::lua::module::file::exist(const std::string& path)
{
	return newobj::file::exist(path);
}

bool newobj::lua::module::file::remove(const std::string& filepath)
{
	return newobj::file::remove(filepath);
}

bool newobj::lua::module::file::write_all(const std::string& filepath, const std::string& data)
{
	return newobj::file::write(filepath,data);
}

bool newobj::lua::module::file::create_dir(const std::string& filepath, bool recursion)
{
	return newobj::file::create_dir(filepath,recursion);
}

bool newobj::lua::module::file::copy_dir(const std::string& src, const std::string& dst, bool cover)
{
	return newobj::file::copy_dir(src, dst,cover);
}

bool newobj::lua::module::file::copy_file(const std::string& src, const std::string& dst, bool cover)
{
	return newobj::file::copy_file(src, dst, cover);
}

bool newobj::lua::module::file::remove_dir(const std::string& dirpath, bool recycle)
{
	return newobj::file::remove_dir(dirpath, recycle);
}

std::string newobj::lua::module::file::pre_dir(const std::string& path)
{
	return newobj::file::pre_dir(path);
}

std::string newobj::lua::module::file::ext(const std::string& path)
{
	return newobj::file::ext(path);
}

std::string newobj::lua::module::file::filename(const std::string& path)
{
	return newobj::file::filename(path);
}

std::string newobj::lua::module::file::format(const std::string& path, bool trailshash)
{
	return newobj::file::format(path, trailshash);
}

void newobj::lua::module::regist_file(void* config, kaguya::State* L)
{
	m_config = (module_config<newobj::lua::module::file>*)config;


	auto table_file = kaguya::UserdataMetatable<newobj::lua::module::file>();
	table_file.setConstructors<newobj::lua::module::file()>();
	table_file.addFunction("open", &newobj::lua::module::file::open);
	table_file.addFunction("write", &newobj::lua::module::file::write);
	table_file.addFunction("jump", &newobj::lua::module::file::jump);
	table_file.addFunction("close", &newobj::lua::module::file::close);
	table_file.addFunction("flush", &newobj::lua::module::file::flush);
	table_file.addFunction("clear", &newobj::lua::module::file::clear);
	table_file.addFunction("filepath", &newobj::lua::module::file::filepath);
	table_file.addFunction("read", &newobj::lua::module::file::read);



	table_file.addStaticFunction("copy_dir", &newobj::lua::module::file::copy_dir);
	table_file.addStaticFunction("copy_file", &newobj::lua::module::file::copy_file);
	table_file.addStaticFunction("create_dir", &newobj::lua::module::file::create_dir);
	table_file.addStaticFunction("exist", &newobj::lua::module::file::exist);
	table_file.addStaticFunction("ext", &newobj::lua::module::file::ext);
	table_file.addStaticFunction("filename", &newobj::lua::module::file::filename);
	table_file.addStaticFunction("format", &newobj::lua::module::file::format);
	table_file.addStaticFunction("pre_dir", &newobj::lua::module::file::pre_dir);
	table_file.addStaticFunction("read_all", &newobj::lua::module::file::read_all);
	table_file.addStaticFunction("remove", &newobj::lua::module::file::remove);
	table_file.addStaticFunction("remove_dir", &newobj::lua::module::file::remove_dir);
	table_file.addStaticFunction("size", &newobj::lua::module::file::size);
	table_file.addStaticFunction("write_all", &newobj::lua::module::file::write_all);

	(*L)["FILE"].setClass(table_file);
}
