#pragma once
extern "C"
{
	#include "core/lua.h"
	#include "core/lauxlib.h"
	#include "core/lualib.h"
}
#include <vector>
#define lua_bind_class_create(classtype,L,classname)										\
	lua_pushcfunction(L, [](lua_State* L)->int {													\
		*(classtype**)lua_newuserdata(L, sizeof(classtype*)) = new classtype();	\
		luaL_getmetatable(L, classname);															\
		lua_setmetatable(L, -2);																\
		return 1;																								\
	});																												\
	lua_setglobal(L, classname);																		\
	luaL_newmetatable(L, classname);																\
	lua_pushvalue(L, -1);																					\
	lua_pushstring(L, "__index");																		\
	lua_pushvalue(L, -2);																					\
	lua_settable(L, -3);		

#define lua_bind_class_destory(classtype,L)													\
	luaL_newmetatable(L, "__gc");																	\
	lua_pushcfunction(L, [](lua_State* L)->int {													\
		delete* (classtype**)lua_topointer(L, 1);													\
		return 0;																								\
	});																												\
	lua_settable(L, -3);																						\


#define lua_define_class_function(functionname)				\
	static int functionname(lua_State* L)

#define lua_bind_class_function(function,lua_funname)									\
	lua_pushstring(L, lua_funname);																\
	lua_pushcfunction(L, function);																	\
	lua_settable(L, -3);																						\
	functions[lua_funname] = function

//#define lua_class_ok																						\
//	luaL_Reg* lualRegPotiner = new luaL_Reg[functions.size()];	\						
//	size_t idx = 0;																							\
//	for (auto iter = functions.begin(); iter != functions.end(); iter++)															\
//	{																												\
//		lualRegPotiner[idx] = {																			\
//			iter->first.c_str(),																				\
//			iter->second																						\
//		};																											\
//		idx++;																									\
//	}																												\
//	luaL_setfuncs(L, lualRegPotiner, 0)

template<typename T>
struct module_config
{
	~module_config()
	{
		for (size_t i = 0; i < m_ptrs.size(); i++)
			delete m_ptrs[i];
	}
	T* create()
	{
		auto p = new T();
		m_ptrs.push_back(p);
		return p;
	}
	std::vector<T*> m_ptrs;
};