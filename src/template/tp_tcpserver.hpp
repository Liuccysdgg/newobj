#pragma once
#include "network/tcp_server.h"
#include "tp_streamproc.hpp"


void _TP_TcpServer_OnAccept(newobj::network::iserver* pServer, uint64 connID);
void _TP_TcpServer_OnClose(newobj::network::iserver* pServer, uint64 connID);
void _TP_TcpServer_OnRecv(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len);


namespace newobj
{
	struct TcpServerProcTPExtra
	{
		newobj::network::iserver* pServer = nullptr;
		uint64 connID = 0;
	};
	/*****************************************************************
	 * Class£ºTcpServer´¦ÀíÄ£°å
	 *****************************************************************/
	//template <typename Extra>
	class itcpserverproc:public network::tcp::server,public istreamproc<TcpServerProcTPExtra>
	{
	public:
		itcpserverproc(network::TcpType tt):network::tcp::server(tt)
		{
			::network::tcp::server::on_recv(_TP_TcpServer_OnRecv);
			::network::tcp::server::on_close(_TP_TcpServer_OnClose);
			::network::tcp::server::on_accept(_TP_TcpServer_OnAccept);
		}
		~itcpserverproc()
		{

		}
		virtual void onClose(newobj::network::iserver* pServer, uint64 connID) = 0;
		virtual void onAccept(newobj::network::iserver* pServer, uint64 connID) = 0;

		network::iserver* get()
		{
			return this;
		}

	private:

	};
}


void _TP_TcpServer_OnAccept(newobj::network::iserver* pServer, uint64 connID)
{
	((itcpserverproc*)pServer)->onAccept(pServer,connID);

}

void  _TP_TcpServer_OnClose(newobj::network::iserver* pServer, uint64 connID)
{
	((itcpserverproc*)pServer)->onClose(pServer, connID);
}
void  _TP_TcpServer_OnRecv(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)
{
	TcpServerProcTPExtra extra;
	extra.pServer = pServer;
	extra.connID = connID;
	((itcpserverproc*)pServer)->post(buf,buf_len,extra);
}