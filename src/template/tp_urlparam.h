#pragma once
#ifndef MSVC_2010
#include "util/forstring.h"
#include <initializer_list>
#include "public/string.h"
namespace newobj
{
	namespace web
	{
		class NEWOBJ_API urlparam
		{
		public:
			urlparam(const std::initializer_list<Kv>& kvs);
			urlparam();
			~urlparam();
			void operator=(const std::initializer_list<Kv>& kvs);
			nstring to_string();
			void append(const Kv& kv);
		private:
			std::vector<Kv> m_kvs;
		};
	}
}



#endif