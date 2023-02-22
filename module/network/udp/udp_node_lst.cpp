//#include "log/log_define.h"
#include "udp_node.h"
#if USE_NET_UDP_NODE
#include "util/forstring.h"
#include "hpsocket/hpsocket.h"
#include <algorithm>
#include "public/exception.h"
#include "udp_node_lst.h"


namespace newobj
{
	namespace network
	{
		namespace udp
		{
			udp_node_lst::udp_node_lst(node* udpNode)
			{
				m_threadpool = NULL;
				this->m_udpNode = udpNode;

				/*是否为性能模式，=true 则开启线程池*/
				if (this->m_udpNode->sport_mode())
				{
					//创建Socket任务线程池
					this->m_threadpool = HP_Create_ThreadPool();
					if (this->m_threadpool->Start(
						0,
						this->m_udpNode->sport_poliy(),
						this->m_udpNode->sport_poliy() == 0 ? TRP_WAIT_FOR : TRP_CALL_FAIL, 0) == false)
					{
						//销毁Socket任务线程池
						HP_Destroy_ThreadPool(this->m_threadpool);
						this->m_threadpool = nullptr;
						trw_str("Threadpool Init failed");
					}
				}
			}
			udp_node_lst::~udp_node_lst()
			{
				if (this->m_threadpool != NULL)
				{
					this->m_threadpool->Stop(60000);
					delete this->m_threadpool;
				}
			}
			EnHandleResult udp_node_lst::OnPrepareListen(IUdpNode* pSender, SOCKET soListen)
			{
				//std::cout << "OnPrepareListen" << std::endl;
				return HR_OK;
			}
			EnHandleResult udp_node_lst::OnSend(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength)
			{
				//std::cout << "OnSend" << std::endl;
				return HR_OK;
			}
			EnHandleResult udp_node_lst::OnReceive(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength)
			{
				//printf(">>>Recv(%d):   ", iLength);
				//for (int i = 0; i < (iLength>10 ? 10 : iLength); i++)
				//{
				//	printf("[%x]", (char)pData[i]);
				//}
				//printf("\n");


				if (this->m_udpNode->m_pfun_onrecv != NULL)
					this->m_udpNode->m_pfun_onrecv(this->m_udpNode, lpszRemoteAddress, usRemotePort, (const char*)pData, iLength);
				return HR_OK;
			}
			EnHandleResult udp_node_lst::OnError(IUdpNode* pSender, EnSocketOperation enOperation, int iErrorCode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength)
			{
				//std::cout << "OnError" << std::endl;
				return HR_OK;
			}
			EnHandleResult udp_node_lst::OnShutdown(IUdpNode* pSender)
			{
				//std::cout << "OnShutdown" << std::endl;
				return HR_OK;
			}
		}
		
	}
}
#endif