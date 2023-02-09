#include "tcp_server.h"
#ifdef USE_NET_TCP_SERVER
#include "public/exception.h"
#include "hpsocket/hpsocket.h"
#include "util/system.h"
#include "tcp_server_lst.h"
#include<string.h>
namespace newobj
{
	namespace network
	{
		namespace tcp
		{
			server::server(TcpType tt)
			{
				this->m_tcptype = tt;
				m_pfun_onaccept = NULL;
				m_pfun_onclose = NULL;
				m_pfun_onrecv = NULL;
				m_pfun_onfilter = NULL;
				m_pfun_onsend = NULL;
				m_pfun_onhandshake = NULL;
				m_pfun_shutdown = NULL;
				m_pfun_ondiscard = NULL;
			}
			server::~server()
			{
				close();
			}
			// 通过 IServer 继承
			bool server::start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode, ushort sport_policy)
			{
				nstring throw_str;
				bool result = false;
				m_sportMode = sportMode;
				m_sport_policy = sport_policy;
				//判断是否已存在服务端
				if (this->m_hp_ptr != 0)
				{
					this->m_lastErrorDesc = "started";
					return false;
				}if (this->m_hp_ptr != 0)
				{
					this->m_lastErrorDesc = "started";
					return false;
				}
				//创建Server
				m_hp_listener_ptr = (ptr)new tcp_server_lst(this);

				if (this->m_tcptype == TTGen)
				{
					//普通TCPServer

					this->m_hp_ptr = (ptr)HP_Create_TcpServer((ITcpServerListener*)m_hp_listener_ptr);
					((ITcpServer*)this->m_hp_ptr)->SetSendPolicy(SP_PACK);
					//((ITcpServer*)this->m_hp_ptr)->SetSocketBufferSize(1024*8);
					//((ITcpServer*)this->m_hp_ptr)->SetSocketBufferSize();
					
					

					result = ((ITcpServer*)this->m_hp_ptr)->Start(bind_ipaddress.c_str(), bind_port);
				}
				else if (this->m_tcptype == TTPack)
				{
					//PACK TCPServer
					this->m_hp_ptr = (ptr)HP_Create_TcpPackServer((ITcpServerListener*)m_hp_listener_ptr);
					result = ((ITcpPackServer*)this->m_hp_ptr)->Start(bind_ipaddress.c_str(), bind_port);
				}
				else if (this->m_tcptype == TTPull)
				{
					//PULL TCPServer
					this->m_hp_ptr = (ptr)HP_Create_TcpPullServer((ITcpServerListener*)m_hp_listener_ptr);
					result = ((ITcpPullServer*)this->m_hp_ptr)->Start(bind_ipaddress.c_str(), bind_port);
				}
				else
				{
					this->m_lastErrorDesc = "Other tcpserver types are not supported";
					goto FREE_;
				}

				//(*(CTcpPackServerPtr*)this->m_hpserver)->Start
				if (result == false)
				{
					this->m_lastErrorDesc = "error code:" + nstring::from((uint32)SYS_GetLastError());
					goto FREE_;
				}
				return true;
			FREE_:
				{
					if (this->m_tcptype == TTGen)
					{
						HP_Destroy_TcpServer(to_ptr(ITcpServer*, m_hp_ptr));
					}
					else if (this->m_tcptype == TTPack)
					{
						HP_Destroy_TcpPackServer(to_ptr(ITcpPackServer*, m_hp_ptr));
					}
					else if (this->m_tcptype == TTPack)
					{
						HP_Destroy_TcpPullServer(to_ptr(ITcpPullServer*, m_hp_ptr));
					}
					//sys::Delete((ITcpPackServer*)m_hp_ptr);
					this->m_hp_ptr = 0;

					return false;
				}


			}

			bool server::close(uint32 wait_msec)
			{
				if (this->m_hp_ptr == 0)
					return false;;
				if (this->m_tcptype == TTGen)
				{
					if (to_ptr(ITcpServer*, hp())->Stop() == false)
					{
						this->m_lastErrorDesc = "stop failed, error code:" + nstring::from((uint32)SYS_GetLastError());
						return false;
					}
					if (to_ptr(ITcpServer*, hp())->Wait(wait_msec == 0 ? -1 : wait_msec) == false)
					{
						this->m_lastErrorDesc = "waiting failed, error code:" + nstring::from((uint32)SYS_GetLastError());
						return false;
					}
					HP_Destroy_TcpServer(to_ptr(ITcpServer*, m_hp_ptr));
				}
				else if (this->m_tcptype == TTPack)
				{
					if (to_ptr(ITcpPackServer*, hp())->Stop() == false)
					{
						this->m_lastErrorDesc = "stop failed, error code:" + nstring::from((uint32)SYS_GetLastError());
						return false;
					}
					if (to_ptr(ITcpPackServer*, hp())->Wait(wait_msec == 0 ? -1 : wait_msec) == false)
					{
						this->m_lastErrorDesc = "waiting failed, error code:" + nstring::from((uint32)SYS_GetLastError());
						return false;
					}
					HP_Destroy_TcpPackServer(to_ptr(ITcpPackServer*, m_hp_ptr));
				}
				else if (this->m_tcptype == TTPull)
				{
					if (to_ptr(ITcpPullServer*, hp())->Stop() == false)
					{
						this->m_lastErrorDesc = "stop failed, error code:" + nstring::from((uint32)SYS_GetLastError());
						return false;
					}
					if (to_ptr(ITcpPullServer*, hp())->Wait(wait_msec == 0 ? -1 : wait_msec) == false)
					{
						this->m_lastErrorDesc = "waiting failed, error code:" + nstring::from((uint32)SYS_GetLastError());
						return false;
					}
					HP_Destroy_TcpPullServer(to_ptr(ITcpPullServer*, m_hp_ptr));
				}
				//sys::Delete((ITcpPackServer*)m_hp_ptr);
				this->m_hp_ptr = 0;
				delete (tcp_server_lst*)this->m_hp_listener_ptr;
				return true;
				//delete 
			}
			void server::send(uint64 connID, const char* pData, uint32 dataLen)
			{
				f_ret(this->m_hp_ptr);

				bool result = false;
				if (this->m_tcptype == TTGen)
				{
					result = to_ptr(ITcpServer*, hp())->Send((CONNID)connID, (const BYTE*)pData, dataLen);
				}
				else if (this->m_tcptype == TTPack)
				{
					result = to_ptr(ITcpPackServer*, hp())->Send((CONNID)connID, (const BYTE*)pData, dataLen);
				}
				else if (this->m_tcptype == TTPull)
				{
					result = to_ptr(ITcpPullServer*, hp())->Send((CONNID)connID, (const BYTE*)pData, dataLen);
				}
				if (result == false)
				{
					trw_str("Send failed,connID:" + nstring::from((uint64)(CONNID)connID) + " error code:" + nstring::from((uint32)SYS_GetLastError()));
				}
			}
			void server::send(uint64 connID, const buffer& data)
			{
				send(connID, (const char*)data.data(), data.length());
			}
			bool server::extra(uint64 connID, ptr extra)
			{
				f_ret_f(this->conned(connID));

				if (this->m_tcptype == TTGen)
				{
					return to_ptr(ITcpServer*, hp())->SetConnectionExtra((CONNID)connID, (PVOID)extra);
				}
				else if (this->m_tcptype == TTPack)
				{
					return to_ptr(ITcpPackServer*, hp())->SetConnectionExtra((CONNID)connID, (PVOID)extra);
				}
				else if (this->m_tcptype == TTPull)
				{
					return to_ptr(ITcpPullServer*, hp())->SetConnectionExtra((CONNID)connID, (PVOID)extra);
				}
				return false;

			}
			ptr server::extra(uint64 connID)
			{

				PVOID extra_ptr = 0;
				if (this->m_tcptype == TTGen)
				{
					if (to_ptr(ITcpServer*, m_hp_ptr)->GetConnectionExtra((CONNID)connID, &extra_ptr) == false)
					{
						return 0;
					}
				}
				else if (this->m_tcptype == TTPack)
				{
					if (to_ptr(ITcpPackServer*, m_hp_ptr)->GetConnectionExtra((CONNID)connID, &extra_ptr) == false)
					{
						return 0;
					}
				}
				else if (this->m_tcptype == TTPull)
				{
					if (to_ptr(ITcpPullServer*, m_hp_ptr)->GetConnectionExtra((CONNID)connID, &extra_ptr) == false)
					{
						return 0;
					}
				}
				return (ptr)extra_ptr;
			}
			bool server::conned(uint64 connID)
			{
				if (this->m_tcptype == TTGen)
				{
					return to_ptr(ITcpServer*, m_hp_ptr)->IsConnected((CONNID)connID);
				}
				else if (this->m_tcptype == TTPack)
				{
					return to_ptr(ITcpPackServer*, m_hp_ptr)->IsConnected((CONNID)connID);
				}
				else if (this->m_tcptype == TTPull)
				{
					return to_ptr(ITcpPullServer*, m_hp_ptr)->IsConnected((CONNID)connID);
				}
				else
				{
					return false;
				}
			}
			bool server::disconn(uint64 connID)
			{
				if (this->m_tcptype == TTGen)
				{
					return to_ptr(ITcpServer*, m_hp_ptr)->Disconnect((CONNID)connID, true);
				}
				else if (this->m_tcptype == TTPack)
				{
					return to_ptr(ITcpPackServer*, m_hp_ptr)->Disconnect((CONNID)connID, true);
				}
				else if (this->m_tcptype == TTPull)
				{
					return to_ptr(ITcpPullServer*, m_hp_ptr)->Disconnect((CONNID)connID, true);
				}
				else
				{
					return false;
				}
			}
			bool server::remote(uint64 connID, nstring& ipaddress, ushort& port)
			{


				int ipAddrLen = 128;
				TCHAR ipAddr[128];
				memset(ipAddr, 128, 0);


				if (this->m_tcptype == TTGen)
				{

					f_ret_f(to_ptr(ITcpServer*, m_hp_ptr)->GetRemoteAddress((CONNID)connID, ipAddr, ipAddrLen, port));
				}
				else if (this->m_tcptype == TTPack)
				{
					f_ret_f(to_ptr(ITcpPackServer*, m_hp_ptr)->GetRemoteAddress((CONNID)connID, ipAddr, ipAddrLen, port));
				}
				else if (this->m_tcptype == TTPull)
				{
					f_ret_f(to_ptr(ITcpPullServer*, m_hp_ptr)->GetRemoteAddress((CONNID)connID, ipAddr, ipAddrLen, port));
				}
				else
				{
					return false;
				}
				ipaddress = ipAddr;
				port = port;
				return true;
			}
			/****************************************************************
			 * Fun：[事件]连接
			 ****************************************************************/
			void server::on_accept(std::function<void(newobj::network::iserver* pServer, uint64 connID)> pfun)
			{
				{
					this->m_pfun_onaccept = pfun;
				}
			}
			/****************************************************************
			 * Fun：[事件]关闭
			 ****************************************************************/
			void server::on_close(std::function<void(newobj::network::iserver* pServer, uint64 connID)> pfun)
			{
				this->m_pfun_onclose = pfun;
			}
			/****************************************************************
			 * Fun：[事件]接收
			 ****************************************************************/
			void server::on_recv(std::function<void(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)> pfun)
			{
				this->m_pfun_onrecv = pfun;
			}
			/****************************************************************
			 * Fun：[事件]握手
			 ****************************************************************/
			void server::on_handshake(std::function<void(newobj::network::iserver* pServer, uint64 connID)> pfun)
			{
				this->m_pfun_onhandshake = pfun;
			}
			/****************************************************************
			 * Fun：[事件]已发送
			 ****************************************************************/
			void server::on_send(std::function<void(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)> pfun)
			{
				this->m_pfun_onsend = pfun;
			}
			/****************************************************************
			 * Fun：[事件]数据包因拒绝策略被丢弃
			 * Desc：仅当开启性能模式并且拒绝策略>0才会触发该函数
			 ****************************************************************/
			void server::on_discard(std::function<void(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)> pfun)
			{
				this->m_pfun_ondiscard = pfun;
			}

			void server::on_shutdown(std::function<void(newobj::network::iserver* pServer)> pfun)
			{
				this->m_pfun_shutdown = pfun;
			}

			void server::on_filter(std::function<newobj::buffer(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)> pfun)
			{
				this->m_pfun_onfilter = pfun;
			}

			void server::on_preparelisten(std::function<void(newobj::network::tcp::server*)> pfun)
			{
				m_pfun_onpreparelisten = pfun;
			}

			int32 server::wait_queue()
			{
				if (m_hp_listener_ptr == 0)
					return -1;
				return to_ptr(tcp_server_lst*, m_hp_listener_ptr)->wait_queue();
			}

			int32 server::proc_total()
			{
				if (m_hp_listener_ptr == 0)
					return -1;
				return to_ptr(tcp_server_lst*, m_hp_listener_ptr)->proc_total();
			}

			uint32 server::conn_count()
			{
				if (m_hp_ptr == 0)
					return 0;

				if (this->m_tcptype == TTGen)
				{
					//普通TCPServer
					return ((ITcpServer*)this->m_hp_ptr)->GetConnectionCount();
				}
				else if (this->m_tcptype == TTPack)
				{
					//PACK TCPServer
					return ((ITcpPackServer*)this->m_hp_ptr)->GetConnectionCount();
				}
				else if (this->m_tcptype == TTPull)
				{
					//PULL TCPServer
					return ((ITcpPullServer*)this->m_hp_ptr)->GetConnectionCount();
				}
				else
				{
					trw_str("Other tcpserver types are not supported");
				}



			}
		}
		
		
	}
}

#endif