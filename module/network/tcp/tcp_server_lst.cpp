#include "tcp_server_lst.h"
#ifdef USE_NET_TCP_SERVER
#include "tcp_server.h"
#include <string.h>

namespace newobj
{
	namespace network
	{
		namespace tcp
		{

			tcp_server_lst::tcp_server_lst(newobj::network::tcp::server* tcpServer)
			{
				m_threadpool = NULL;
				this->m_tcpServer = tcpServer;
				/*是否为性能模式，=true 则开启线程池*/
				if (this->m_tcpServer->sport_mode())
				{
					//创建Socket任务线程池
					this->m_threadpool = HP_Create_ThreadPool();
					if (this->m_threadpool->Start(
						(system::hardware::cpu().number * 2 + 2) * 2,
						this->m_tcpServer->sport_poliy(),
						this->m_tcpServer->sport_poliy() == 0 ? TRP_WAIT_FOR : TRP_CALL_FAIL, 0) == false)
					{
						//销毁Socket任务线程池
						HP_Destroy_ThreadPool(this->m_threadpool);
						this->m_threadpool = nullptr;
						trw_str("Threadpool Init failed");
					}
				}


			}
			tcp_server_lst::~tcp_server_lst()
			{
				if (this->m_threadpool != NULL)
				{
					if (!this->m_threadpool->Stop(60000))
					{
						//trw_STR("ThreadPool Stop failed");
					}
					delete this->m_threadpool;
				}
			}
			EnHandleResult tcp_server_lst::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
			{
				/*回调*/
				if (m_tcpServer->m_pfun_onpreparelisten != NULL)
					m_tcpServer->m_pfun_onpreparelisten(this->m_tcpServer);
				return HR_OK;
			}
			EnHandleResult tcp_server_lst::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
			{
				/*回调*/
				if (m_tcpServer->m_pfun_onsend != NULL)
					m_tcpServer->m_pfun_onsend(this->m_tcpServer, dwConnID, (const char*)pData, iLength);
				return HR_OK;
			}
			EnHandleResult tcp_server_lst::OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
			{
				_onReceive(pSender, dwConnID, pData, iLength);
				return HR_OK;
			}
			EnHandleResult tcp_server_lst::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
			{
				/*回调*/
				if (m_tcpServer->m_pfun_onclose != NULL)
					m_tcpServer->m_pfun_onclose(this->m_tcpServer, dwConnID);
				return HR_OK;
			}

			EnHandleResult tcp_server_lst::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
			{
				/*回调*/
				if (m_tcpServer->m_pfun_onaccept != NULL)
					m_tcpServer->m_pfun_onaccept(this->m_tcpServer, dwConnID);
				return HR_OK;
			}
			EnHandleResult tcp_server_lst::OnShutdown(ITcpServer* pSender)
			{
				/*回调*/
				if (m_tcpServer->m_pfun_shutdown != NULL)
					m_tcpServer->m_pfun_shutdown(this->m_tcpServer);
				return HR_OK;
			}
			EnHandleResult tcp_server_lst::OnHandShake(ITcpServer* pSender, CONNID dwConnID)
			{
				/*回调*/
				if (m_tcpServer->m_pfun_onhandshake != NULL)
					m_tcpServer->m_pfun_onhandshake(this->m_tcpServer, dwConnID);
				return HR_OK;
			}

			int32 tcp_server_lst::wait_queue()
			{
				if (this->m_threadpool == NULL)
					return -1;
				return this->m_threadpool->GetQueueSize();
			}

			int32 tcp_server_lst::proc_total()
			{
				if (this->m_threadpool == NULL)
					return -1;
				return this->m_threadpool->GetTaskCount();
			}

			void tcp_server_lst::_onReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
			{
				if (iLength <= 0)
					return;
				newobj::buffer buf;
				if (this->m_tcpServer->m_pfun_onfilter != nullptr)
				{
					buf = this->m_tcpServer->m_pfun_onfilter(this->m_tcpServer, dwConnID, (const char*)pData, iLength);
					if (buf.data() == nullptr)
						return;
				}
				else
				{
					buf = buffer((char*)pData, iLength);
				}


				/*回调*/
				if (m_tcpServer->m_pfun_onrecv != NULL)
				{
					/*是否为性能模式，选择是否使用线程*/
					if (this->m_tcpServer->m_sportMode)
					{

						TcpServerRecvSt* tsrs = new TcpServerRecvSt;

						if (this->m_tcpServer->m_pfun_onfilter == nullptr)
						{
							tsrs->pData = new char[iLength];
							memcpy(tsrs->pData, (const char*)pData, iLength);
							tsrs->iLength = iLength;
						}
						else
						{
							tsrs->pData = (char*)pData;
							tsrs->iLength = iLength;
						}


						tsrs->dwConnID = dwConnID;


						tsrs->pSender = pSender;
						tsrs->pServer = (iserver*)this->m_tcpServer;
						tsrs->callback = this->m_tcpServer->m_pfun_onrecv;
						/*启动线程池*/
						if (this->m_threadpool->Submit(&callback_thread_recv_tcp, (PVOID)tsrs) == false)
						{

							//选择是否触发失败回调
							if (this->m_tcpServer->sport_poliy() > 0)
							{
								if (this->m_tcpServer->m_pfun_ondiscard != NULL)
									this->m_tcpServer->m_pfun_ondiscard(this->m_tcpServer, dwConnID, tsrs->pData, tsrs->iLength);
							}

							//失败释放资源
							if (tsrs->pData != NULL)
								delete[] tsrs->pData;
							delete tsrs;
						}
					}
					else
					{
						m_tcpServer->m_pfun_onrecv(this->m_tcpServer, dwConnID, (char*)buf.data(), buf.length());
					}
				}
			}

			EnHandleResult tcp_server_lst::OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength)
			{
				_onReceive(pSender, dwConnID, NULL, iLength);
				return HR_OK;
			}

			void __HP_CALL callback_thread_recv_tcp(PVOID param)
			{
				TcpServerRecvSt* tsrs = (TcpServerRecvSt*)param;
				tsrs->callback(tsrs->pServer, tsrs->dwConnID, tsrs->pData, tsrs->iLength);
				if (tsrs->pData != NULL)
					delete[] tsrs->pData;
				delete tsrs;
			}
		}
	}
}
#endif