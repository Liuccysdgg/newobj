#pragma once

#include "public/var.h"
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
		bool extra(const nstring& name, const nvar& data);
		nvar extra(const nstring& name);
	private:
		map<nstring, nvar> m_userData;
		newobj::mutex m_mutex;
	};
}
