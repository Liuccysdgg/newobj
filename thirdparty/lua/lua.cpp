#include "lua.h"
#include "module/define.h"
#include "module/file.h"
#include "module/json.h"
#include "module/ini.h"
#include "module/system.h"
#include "module/time.h"
#include "module/vector_int.h"
#include "module/vector_string.h"
#include "module/forstring.h"
#include "kaguya/kaguya.hpp"
newobj::json script_exec(const nstring& script, const nstring& function, const newobj::json& param, nstring& error_msg)
{
	kaguya::State state;
	if (state.dostring(script) == false)
	{
		error_msg = "dostring failed";
		return newobj::json();
	}
	module_config<newobj::lua::module::file> file_config;
	module_config<newobj::lua::module::json> json_config;
	module_config<newobj::lua::module::ini> ini_config;
	module_config<newobj::lua::module::system> system_config;
	module_config<newobj::lua::module::time> time_config;
	module_config<newobj::lua::module::vector_int> vector_int_config;
	module_config<newobj::lua::module::vector_string> vector_string_config;
	module_config<newobj::lua::module::forstring> forstring_config;

	newobj::lua::module::regist_file(&file_config, &state);
	newobj::lua::module::regist_json(&json_config, &state);
	newobj::lua::module::regist_ini(&ini_config, &state);
	newobj::lua::module::regist_system(&system_config, &state);
	newobj::lua::module::regist_time(&time_config, &state);
	newobj::lua::module::regist_vector(&vector_int_config, &state);
	newobj::lua::module::regist_vector_string(&vector_string_config, &state);
	newobj::lua::module::regist_forstring(&forstring_config, &state);

	return newobj::json::from(state[function.data()].call<std::string>(param.to_string().data()));
}
