#pragma once
#include "network/define.h"
#ifdef USE_NET_TCP_SERVER
#include "network/iserver.h"
#include <string>
#include "public/define.h"
#include "hpsocket/hpsocket.h"
#include "public/exception.h"
#include "util/system.h"
#include "util/map.hpp"
#include "public/buffer.h"
#include <functional>
namespace newobj
{
	namespace network
	{
		namespace tcp
		{
			
			typedef void(*CALLBACK_TCPSERVER_ONACCEPT)(newobj::network::iserver* pServer, uint64 connID);
			typedef CALLBACK_TCPSERVER_ONACCEPT CALLBACK_TCPSERVER_ONCLOSE;
			typedef CALLBACK_TCPSERVER_ONACCEPT CALLBACK_TCPSERVER_ONHANDSHAKE;
			typedef void(*CALLBACK_TCPSERVER_ONRECV)(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len);
			typedef buffer(*CALLBACK_TCPSERVER_ONFILTER)(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len);
			typedef CALLBACK_TCPSERVER_ONRECV CALLBACK_TCPSERVER_ONDISCARD;
			typedef CALLBACK_TCPSERVER_ONRECV CALLBACK_TCPSERVER_ONSEND;
			typedef void(*CALLBACK_TCPSERVER_ONSHUTDOWN)(newobj::network::iserver* pServer);

			class srvlistener;
			/****************************************************************
			 * Class: TCP服务器
			 * Desc：使用HPSocket开源库。
			 ****************************************************************/
			class MNETWORK_API server :public iserver
			{
			public:
				server(TcpType tt);
				~server();
				// 通过 IServer 继承
				virtual bool start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode = false, ushort sport_policy = 10000) override;
				virtual bool close(uint32 wait_msec = 0) override;
				virtual void send(uint64 connID, const char* pData, uint32 dataLen) override;
				virtual void send(uint64 connID, const buffer& data) override;
				virtual bool extra(uint64 connID, ptr extra) override;
				virtual ptr extra(uint64 connID) override;
				virtual bool conned(uint64 connID) override;
				virtual bool disconn(uint64 connID) override;
				virtual bool remote(uint64 connID, nstring& ipaddress, ushort& port) override;
				virtual int32 wait_queue() override;

				virtual int32 proc_total() override;
				virtual uint32 conn_count() override;
				/****************************************************************
				 * Fun：[事件]连接
				 ****************************************************************/
				void on_accept(std::function<void(newobj::network::iserver* pServer, uint64 connID)> pfun);
				/****************************************************************
				 * Fun：[事件]关闭
				 ****************************************************************/
				void on_close(std::function<void(newobj::network::iserver* pServer, uint64 connID)> pfun);
				/****************************************************************
				 * Fun：[事件]接收
				 ****************************************************************/
				void on_recv(std::function<void(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)> pfun);
				/****************************************************************
				 * Fun：[事件]握手
				 ****************************************************************/
				void on_handshake(std::function<void(newobj::network::iserver* pServer, uint64 connID)> pfun);
				/****************************************************************
				 * Fun：[事件]已发送
				 ****************************************************************/
				void on_send(std::function<void(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)> pfun);
				/****************************************************************
				 * Fun：[事件]数据包因拒绝策略被丢弃
				 * Desc：仅当开启性能模式并且拒绝策略>0才会触发该函数
				 ****************************************************************/
				void on_discard(std::function<void(newobj::network::iserver* pServer, uint64 connID,const char* buf, uint32 buf_len)> pfun);
				/****************************************************************
				 * Fun：[事件]停止
				 ****************************************************************/
				void on_shutdown(std::function<void(newobj::network::iserver* pServer)> pfun);
				/****************************************************************
				 * Fun：[事件]过滤接收
				 ****************************************************************/
				void on_filter(std::function<newobj::buffer(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)> pfun);
				/****************************************************************
				 * Fun：[事件] 即将启动
				 ****************************************************************/
				void on_preparelisten(std::function<void(newobj::network::tcp::server*)> pfun);


				friend class tcp_server_lst;


			private:
				//回调函数指针
				std::function<void(newobj::network::iserver*, uint64)> m_pfun_onaccept;
				std::function<void(newobj::network::iserver*, uint64)> m_pfun_onclose ;
				std::function<void(newobj::network::iserver*, uint64, const char*, uint32)> m_pfun_onrecv;
				std::function<newobj::buffer(newobj::network::iserver* pServer, uint64, const char*, uint32)> m_pfun_onfilter;
				std::function<void(newobj::network::iserver*, uint64, const char*, uint32)> m_pfun_onsend ;
				std::function<void(newobj::network::iserver*, uint64)> m_pfun_onhandshake ;
				std::function<void(newobj::network::iserver*)> m_pfun_shutdown ;
				std::function<void(newobj::network::iserver*, uint64, const char*, uint32)> m_pfun_ondiscard ;
				std::function<void(newobj::network::tcp::server*)> m_pfun_onpreparelisten;

				TcpType m_tcptype;
			};
		}
		
	}

}

#endif