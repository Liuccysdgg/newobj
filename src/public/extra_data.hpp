#pragma once

#include "public/var.hpp"
#include "public/define.h"
#include "public/string.h"
#include "util/map.hpp"
namespace newobj
{
	/*
	*  ¸½¼ÓÊý¾Ý
	*/
	class extra_data
	{
	public:
        template<typename T>
        bool extra(const nstring& name, const newobj::var<T>& data){
            newobj::var<T> *temp_var = nullptr;
            object* temp_obj = nullptr;
            m_map.lock();
            if(m_map.get(name,temp_obj,false)){
                delete (newobj::var<T>*)temp_obj;
            }
            temp_var = new newobj::var<T>();
            *temp_var = data;
            m_map.add(name,temp_var,false);
            m_map.unlock();
        }
        template<typename T>
        newobj::var<T> extra(const nstring& name){
            object* temp_obj = nullptr;
            if(m_map.get(name,temp_obj) ==false)
                return newobj::var<T>();
            return *((newobj::var<T>*)temp_obj);

        }
	private:
        newobj::map<nstring,object*> m_map;
	};
}
