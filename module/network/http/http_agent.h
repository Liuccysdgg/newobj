#pragma once
#include "public/string.h"
#include "public/base.h"
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
			class MNETWORK_API agent:public newobj::base
			{
			public:
				agent();
				~agent();
				bool start();
				bool stop();
				void disconnect(bool ssl,uint64 connid);
				bool request(int32 wait_msec,reqpack* rp,network::http::proxy* proxy);
			private:
				void* get();
			private:
				void* m_agent_ssl;
				void* m_agent;
				http_agent_listener* m_listener;
				bool m_ssl;
			};
		}
	}
}
#endif
