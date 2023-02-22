#include "util/config.h"
#include "define.h"
#include <map>
#include "verify.h"
#include "util/verify.h"
newobj::lua::module::verify::verify()
{
}

newobj::lua::module::verify::~verify()
{
}


void newobj::lua::module::regist_verify(void* config, kaguya::State* L)
{
	//auto table = kaguya::UserdataMetatable<class newobj::lua::module::verify>();
	//table.addStaticFunction("is_num", [](const std::string& value)- {
	//	return newobj::time::network_msec();
	//});
	//table.addStaticFunction("to_ts", [](const std::string& timestr,const std::string& formart)->long long {
	//	return newobj::time::to_ts(timestr,formart);
	//});
	//table.addStaticFunction("now_sec", []()->long long {
	//	return newobj::time::now_sec();
	//	});
	//table.addStaticFunction("now_msec", []()->long long {
	//	return newobj::time::now_msec();
	//	});
	//table.addStaticFunction("now_usec", []()->long long {
	//	return newobj::time::now_usec();
	//});
	//table.addStaticFunction("now_time", [](const std::string& format)->std::string {
	//	return newobj::time::now_time(format);
	//});
	//table.addStaticFunction("format", [](long long time,const std::string& format)->std::string {
	//	return newobj::time::format(time,format);
	//});
	//table.addStaticFunction("now_zone", []()->int {
	//	return newobj::time::now_zone();
	//});
	//table.addStaticFunction("now_gmt", []()->std::string {
	//	return newobj::time::now_gmt();
	//	});
	//table.addStaticFunction("to_gmt", [](long long time)->std::string {
	//	return newobj::time::to_gmt(time);
	//	});
	//table.addStaticFunction("now_zero_sec", []()->long long {
	//	return newobj::time::now_zero_sec();
	//	});
	//table.addStaticFunction("now_zero_time", []()->std::string {
	//	return newobj::time::now_zero_time();
	//	});
	//(*L)["TIME"].setClass(table);
}

