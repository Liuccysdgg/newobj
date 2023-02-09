#pragma once
#include "network/define.h"
#ifdef USE_NET_TCP_AGENT
#include "hpsocket/hpsocket.h"
#include <string.h>
#include "util/map.hpp"
#include "util/timeout.h"
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
            
            class agent;
            class tcp_agent_lst :public ITcpAgentListener
            {
            public:
                tcp_agent_lst();
                ~tcp_agent_lst();
            private:
				// ͨ�� ITcpAgentListener �̳�
				virtual EnHandleResult OnHandShake(ITcpAgent* pSender, CONNID dwConnID) override;
				virtual EnHandleResult OnSend(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
				virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
				virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, int iLength) override;
				virtual EnHandleResult OnClose(ITcpAgent* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;
				virtual EnHandleResult OnShutdown(ITcpAgent* pSender) override;
				virtual EnHandleResult OnPrepareConnect(ITcpAgent* pSender, CONNID dwConnID, SOCKET socket) override;
                virtual EnHandleResult OnConnect(ITcpAgent* pSender, CONNID dwConnID) override;
            public:
                network::tcp::agent* m_agent;
                newobj::map<uint64, int/*0=������ 1=������ 2=ʧ��*/> m_map;
                newobj::timeout m_timeout;

            };
        }
       
	}
}
#endif