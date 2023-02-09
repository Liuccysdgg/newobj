#pragma once
#include "network/define.h"
#ifdef USE_NET_TCP_AGENT
#include "network/iclient.h"
#include <functional>
#include "hpsocket/hpsocket.h"
#include "tcp_agent_lst.h"
namespace newobj
{
	namespace network
	{
		namespace tcp
		{
            namespace forward
            {
                class server;
            }
            class MNETWORK_API agent
            { 
            public:
                agent();
                ~agent();
                // Í¨¹ý IClient ¼Ì³Ð
                bool start(const nstring& address, bool async);
                bool disconn(uint64 connid);
                bool disconn_longconnections(uint32 msec);
                
                bool close(uint32 wait_msec = 0);
                bool connect(const nstring& address, uint32 port, int32 wait_msec,uint64& connid,ptr extra);
                bool conned(uint64 connid);
                bool send(const char* buff, uint32 len, uint64 connid);
                bool send(const buffer& data, uint64 connid);
                bool extra(uint64 connid,ptr data);

                ptr extra(uint64 connid);
            public:
                void on_recv(std::function<void(network::tcp::agent*,const uint64 &connid,const BYTE* data,uint32 length)> cb);
                void on_accept(std::function<void(network::tcp::agent*, const uint64& connid)> cb);
                void on_close(std::function<void(network::tcp::agent*, const uint64& connid)> cb);
            public:
                network::tcp::forward::server* m_server;
            private:
                friend class network::tcp::tcp_agent_lst;
                std::function<void(network::tcp::agent*, const uint64& connid)> m_pfun_onaccept;
                std::function<void(network::tcp::agent*, const uint64& connid)> m_pfun_onclose;
                std::function<void(network::tcp::agent*, const uint64& connid, const BYTE* data, uint32 length)> m_pfun_onrecv;
                ITcpAgent* m_agent;
                network::tcp::tcp_agent_lst* m_listener;
                
            };
		}
	}
}
#endif