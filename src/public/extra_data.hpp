#pragma once

#include "public/var.hpp"
#include "public/define.h"
#include "public/string.h"
#include "util/map.hpp"
#if defined(_WIN32) && defined(MSVC_2010)
#define EXTRA_DATA_VALUE nstring
#else
#define EXTRA_DATA_VALUE std::any
#endif
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
        void extra(const nstring& name, const EXTRA_DATA_VALUE& data){
            m_map.set(name,data,true);
        }
#ifndef MSVC_2010
        template<typename T>
        T 
#endif
        extra(const nstring& name){
            EXTRA_DATA_VALUE data;
            m_map.get(name, data);
#ifdef MSVC_2010
            return data;
#else
            return std::any_cast<T>(data);
#endif
        }
	private:
        newobj::map<nstring,std::any> m_map;
	};
}
