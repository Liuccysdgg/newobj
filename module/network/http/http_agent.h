#pragma once
#include "basetype/string.h"
#include "public/base.h"
#include "http_define.h"
#include "util/map.hpp"
#include "util/object_pool.hpp"
#if USE_NET_HTTP_WEBSITE
class http_agent_listener;
class http_agent_extra;
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
				void stop();
				void disconnect(bool ssl,uint64 connid);
				bool request(int32 wait_msec, reqpack* rp, network::http::proxy* proxy);
			public:
				newobj::object_pool<http_agent_extra>* m_extra_queue;
				
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
