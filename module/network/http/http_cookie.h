#pragma once
#include "http_define.h"
#if USE_NET_HTTP_UTIL
#include <map>
#include "public/string.h"
namespace newobj
{
	namespace network
	{
		namespace http
		{
			class MNETWORK_API cookie
			{
			public:
				cookie();
				~cookie();
				void merge(const cookie& ck);
				void merge(const nstring& ck);
				nstring to_string();
				void clear();
			private:
				std::map<nstring, nstring> m_param;
			};
		}

	}
	
}
#endif