#include "util/config.h"
#include "define.h"
#include <map>
#include "system.h"
#include "util/system.h"
newobj::lua::module::system::system()
{
}

newobj::lua::module::system::~system()
{
}


void newobj::lua::module::regist_system(void* config, kaguya::State* L)
{
	auto table = kaguya::UserdataMetatable<class newobj::lua::module::system>();

	table.addStaticFunction("sleep_msec", [](int msec) {
		newobj::system::sleep_msec(msec);
	});
	
	table.addStaticFunction("current_dir", []()->std::string {
		return newobj::system::current_dir();
	});
	table.addStaticFunction("current_filepath", []()->std::string {
		return newobj::system::current_filepath();
		});
	table.addStaticFunction("currentuser_path", []()->std::string {
		return newobj::system::currentuser_path();
		});
	table.addStaticFunction("random", [](int min,int max,int rand_idx)->int {
		return newobj::system::random(min,max,rand_idx);
	});
#ifdef _WIN32
	table.addStaticFunction("desktop_path", []()->std::string {
		return newobj::system::desktop_path();
	});

	table.addStaticFunction("clipboard", [](const std::string& value) {
		return newobj::system::clipboard(value);
	});
#endif
	table.addStaticFunction("screen_device_size", []()->std::tuple<int,int> {
		auto size = newobj::system::screen::device_size();
		return std::tuple<int, int>(size.width,size.height);
	});
	table.addStaticFunction("screen_work_size", []()->std::tuple<int, int> {
		auto size = newobj::system::screen::work_size();
		return std::tuple<int, int>(size.width, size.height);
	});
	table.addStaticFunction("screen_set", [](int width,int height) {
		Size size;
		size.width = width;
		size.height = height;
		newobj::system::screen::set(size);
	});
	table.addStaticFunction("mac", []()->std::string {
		return newobj::system::netcard::mac();
	});

	(*L)["SYS"].setClass(table);
}

