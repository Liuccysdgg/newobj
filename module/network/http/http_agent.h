#pragma once
#include "public/string.h"
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
class http_agent_listener;
namespace newobj
{
	namespace network
	{
		namespace http
		{
			class reqpack;
			class server;
			class MNETWORK_API agent
			{
			public:
				agent();
				~agent();
				bool start();
				bool stop();
				bool connect(const nstring& ipaddress,ushort port, int32 wait_msec, uint64& connid,uint64 server_connid,network::http::server* server);
				void disconnect(uint64 connid);
				bool send(network::http::proxy* proxy, uint64 agent_connid, reqpack* rp);
				bool is_connected(uint64 connid);
				uint64 connect_count();
			private:
				void* m_agent;
				http_agent_listener* m_listener;
				bool m_ssl;
			};
		}
	}
}
#endif