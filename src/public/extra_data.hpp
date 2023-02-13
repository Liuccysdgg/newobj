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
        extra_data()
        {

        }
        ~extra_data()
        {

        }
        void extra(const nstring& name, const std::any& data){
            m_map.set(name,data,true);
        }
        template<typename T>
        T extra(const nstring& name){
            std::any data;
            m_map.get(name, data);
            return std::any_cast<T>(data);
        }
	private:
        newobj::map<nstring,std::any> m_map;
	};
}
