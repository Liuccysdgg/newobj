#pragma once
#include "network/define.h"
#ifdef USE_NET_TCP_SERVER
#include "hpsocket/hpsocket.h"
#include "public/define.h"
#include "tcp_server.h"
namespace newobj
{
    namespace network
    {
        namespace tcp
        {
            /*接收数据结构*/
            struct TcpServerRecvSt
            {
                TcpServerRecvSt()
                {
                    dwConnID = 0;
                    pSender = NULL;
                    pServer = NULL;
                    callback = NULL;
                }
                uint64 dwConnID;
                newobj::buffer data;
                ITcpServer* pSender ;
                newobj::network::iserver* pServer ;
                std::function<void(newobj::network::iserver*, uint64, const char*, uint32)> callback ;
            };
            /*接收数据的线程池调用线程，用于再调用用户提供的指针，主要是考虑到 m_sportMode 模式才会使用到该函数*/
            void __HP_CALL callback_thread_recv_tcp(PVOID param);
            /****************************************************************
             * Class: TCP服务器监听器
             ****************************************************************/
            class tcp_server_lst :public CTcpServerListener
            {
            public:
                tcp_server_lst(newobj::network::tcp::server* tcpServer);
                ~tcp_server_lst();
                virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);
                virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
                virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
                virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
                virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);
                virtual EnHandleResult OnShutdown(ITcpServer* pSender);
                virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID);
                virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength);
                int32 wait_queue();
                int32 proc_total();
            private:
                void _onReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLengt);
            public:
                IHPThreadPool* m_threadpool ;
                newobj::network::tcp::server* m_tcpServer ;
            };
        }
        
    }
}
#endif
