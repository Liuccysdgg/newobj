#pragma once
#include "network/define.h"
#if USE_NET_UDP_NODE
#include "hpsocket/hpsocket.h"
#include <string>


namespace newobj
{
    namespace network
    {
        namespace udp
        {
            class node;
            class udp_node_lst :public CUdpNodeListener
            {
            public:
                udp_node_lst(node* udpNode);
                ~udp_node_lst();
            private:
                virtual EnHandleResult OnPrepareListen(IUdpNode* pSender, SOCKET soListen);
                virtual EnHandleResult OnSend(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
                virtual EnHandleResult OnReceive(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
                virtual EnHandleResult OnError(IUdpNode* pSender, EnSocketOperation enOperation, int iErrorCode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength);
                virtual EnHandleResult OnShutdown(IUdpNode* pSender);
            private:
                IHPThreadPool* m_threadpool;
                node* m_udpNode;
            };
        }


    }
}

#endif