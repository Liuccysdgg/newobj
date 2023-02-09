#pragma once
#include "network/define.h"
#if USE_NET_HTTP_UTIL
#include <map>
#include "public/string.h"
#include "public/struct.h"
#include "util/forstring.h"

namespace newobj
{
	namespace network
	{
		namespace http
		{
			
			class MNETWORK_API header
			{
			public:
				header();
				~header();
				void clear();
				void name(const nstring& value);
                nstring name() const;

				std::map<nstring,nstring> param();

                void parse(const nstring& body);
				void push(const nstring& value);
                void push(const nstring& key,const nstring& value);

                bool exist(const nstring& key);

				nstring to_string() const;
			private:
				nstring m_name;
				std::map<nstring, nstring> m_param;
                std::vector<nstring> m_values;
			};
			class MNETWORK_API header_list
			{
			public:
				header_list();
				~header_list();
				bool exist(const nstring& name) const;
				newobj::network::http::header get(const nstring& name);
				void set(const nstring& name, const newobj::network::http::header& header);
				void set(const nstring& name,const nstring& value);
				void del(const nstring& name);
				std::map<nstring, nstring> to() const;
				uint32 size();
				void clear();
			private:
				std::map<nstring, header> m_headers;
			};
		}

	}
	
}
#endif