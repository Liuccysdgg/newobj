#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <string>
#include <vector>
#include <list>

#include "http_header.h"
#include "util/forstring.h"
#include "util/json.h"
#include "http_interface.h"
namespace newobj
{
	namespace network
	{
		
		namespace http
		{
            class server;
            class reqpack;
            class MNETWORK_API response:public network::http::interface_
			{
			public:
				response();
				~response();
                void init(reqpack* rp);
				bool send(const char* buf, size_t buf_len, ushort stateNum = 200, const nstring& stateDesc = "OK");
				bool send(const nstring& value, ushort stateNum = 200, const nstring& stateDesc = "OK");
				bool send(const newobj::json & json, ushort stateNum = 200, const nstring& stateDesc = "OK");
				bool send_file(const nstring& filepath, int32 downbaud = -1);
				std::map<nstring,nstring>* headers();
				bool redirect(const nstring& filepath, bool MovedPermanently = false);
				bool forward(const nstring& filepath);
			public:
				newobj::json sjson;
			private:
				bool filecache(const uint64& last_modify_time);
                bool fileoffset(long filesize, long& start, long& len);
			private:
                std::map<nstring,nstring> m_headers;
                bool m_response;
                network::http::reqpack *m_reqpack;

			};

		}
	}
}

#endif