#pragma once

#include "public/var.hpp"
#include "public/define.h"
#include "public/string.h"
#include "util/map.hpp"
#include <any>
namespace newobj
{
	/*
	*  ¸½¼ÓÊý¾Ý
	*/
	class extra_data
	{
	public:
        template<typename T>
        bool extra(const nstring& name, const std::any& data){
            m_map.set(name,data,true);
        }
        template<typename T>
        std::any extra(const nstring& name){
            m_map.get(name, data);
            return data;
        }
	private:
        newobj::map<nstring,std::any> m_map;
	};
}
