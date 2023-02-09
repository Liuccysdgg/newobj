#pragma once
#include "http_define.h"
#if USE_NET_HTTP_UTIL
#include "public/define.h"
#include "public/string.h"
#include "public/buffer.h"

#include "util/vector.hpp"
namespace newobj
{
	namespace network
	{
		namespace http
		{
			class MNETWORK_API make_form
			{
			public:
				make_form();
				~make_form();
				bool add(const nstring& name, const nstring& value);
				bool add(const nstring& name, const nstring& filename, const nstring& content_type, const newobj::buffer& data);
				bool make(newobj::buffer& data,nstring& boundary) const;
			private:
                newobj::vector<form_info*> m_list;
				newobj::buffer m_data;
			};
		}
	}
}
#endif