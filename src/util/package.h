#pragma once
#if 0
#include "public/var.h"
#include "public/buffer.h"
#include "util/map.hpp"
namespace newobj
{
	
	class package
	{
	private:
		struct packstruct
		{
			packstruct()
			{
				var = true;
			}
			bool var;
			nstring name;
			nstring password;
			nvar data_var;
			newobj::buffer data_buffer;
		};
	public:
		package();
		~package();
		bool parse(const newobj::buffer& data, const nstring& password);
		bool add(const nstring& name,const nvar &data,const nstring& password = "");
		bool add(const nstring& name, const newobj::buffer& data, const nstring& password = "");
		void to(const nstring& password,newobj::buffer& data);
		bool get(const nstring& name, nvar& data, const nstring& password = "");
		bool get(const nstring& name, newobj::buffer& data, const nstring& password = "");
		void setdefaultpassword(const nstring& password);
		void clear();
	private:
		void free();
	private:
		newobj::map<nstring, packstruct*> m_list;
		newobj::map<nstring, newobj::buffer*> m_parse_list;
		nstring m_defaultpassword;
	};
}

#endif
