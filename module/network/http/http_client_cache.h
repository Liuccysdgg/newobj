#pragma once
#include "http_define.h"
#if USE_NET_HTTP_CLIENT
#include "public/string.h"
#include "public/buffer.h"
namespace newobj
{
	namespace network
	{
		namespace http
		{
			class client;
			class client_plus;
			/*¿Í»§¶Ë»º´æ*/
			class MNETWORK_API client_cache
			{
			public:
				client_cache();
				~client_cache();
				void open(const nstring& dirpath);
				bool read(newobj::network::http::client* client,newobj::buffer& cache);
				bool read(newobj::network::http::client_plus* client, newobj::buffer& cache);
				void set_header(newobj::network::http::client* client,const nstring& url);
				void set_header(newobj::network::http::client_plus* client, const nstring& url);
				void close();
				void write(newobj::network::http::client* client);
				void write(newobj::network::http::client_plus *client);
				bool read(const nstring& url,newobj::buffer& cache);
			private:
				bool m_open;
				nstring m_dirpath;
			};
		}
	}
}
#endif