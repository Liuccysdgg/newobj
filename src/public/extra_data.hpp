#pragma once

#include "public/var.hpp"
#include "public/define.h"
#include "public/string.h"
#include "util/map.hpp"
#ifdef MSVC_2010
#define EXTRA_DATA_VALUE nstring
#else
#define EXTRA_DATA_VALUE std::any
#endif

#ifndef MSVC_2010
#include <any>
#endif
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
        void set_extra(const nstring& name, const EXTRA_DATA_VALUE& data){
            m_map.set(name,data,true);
        }
#ifndef MSVC_2010
        template<typename T>
        T 
#else
        EXTRA_DATA_VALUE
#endif
        get_extra(const nstring& name){
            EXTRA_DATA_VALUE data;
            m_map.get(name, data);
#ifdef MSVC_2010
            return data;
#else
            return std::any_cast<T>(data);
#endif
        }
	private:
        newobj::map<nstring, EXTRA_DATA_VALUE> m_map;
	};
}
