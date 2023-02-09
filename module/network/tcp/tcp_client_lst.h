#pragma once
#include "network/define.h"
#ifdef USE_NET_TCP_CLIENT
#include "hpsocket/hpsocket.h"
#include <string.h>
namespace newobj
{
	namespace network
	{
        namespace tcp
        {
            /*接收数据的线程池调用线程，用于再调用用户提供的指针，主要是考虑到 m_sportMode 模式才会使用到该函数*/
            void __HP_CALL callback_thread_recv_tcpclient(PVOID param);

            class client;
            class tcp_client_lst :public CTcpClientListener
            {
            public:
                tcp_client_lst(client* tcpClient);
                ~tcp_client_lst();
            private:

                virtual EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override;
                virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override;
                virtual EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override;
                virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
                virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
                virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;
                virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) override;

            public:
                int m_state;
            private:
                client* m_tcpClient;
                IHPThreadPool* m_threadpool;
            };
        }
       
	}
}
#endif