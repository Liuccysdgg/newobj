#include "http_agent.h"
#if USE_NET_HTTP_WEBSITE
#include "http_reqpack.h"
#include "hpsocket/hpsocket.h"
//#include "hpsocket/windows/include/ssl/HPSocket-SSL.h"
#include "util/system.h"
#include "util/time.h"
#include "util/timeout.h"
#include "util/map.hpp"
#include "http_server.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
struct AutoFreeAgentConnID_
{
	uint64 connid;
	newobj::map<uint64, int>* map;
};
struct HttpAgentExtra
{
	HttpAgentExtra()
	{
		connid = 0;
		server = nullptr;
		transfer_encoding_length = 0;
	}
	uint64 connid;
	network::http::server* server;
	newobj::buffer recv;
	int32 transfer_encoding_length;
};

#define AGENT ((IHttpAgent*)m_agent)
class http_agent_listener :public IHttpAgentListener
{
	// 通过 IHttpAgentListener 继承
	virtual EnHttpParseResult OnMessageBegin(IHttpAgent* pSender, CONNID dwConnID) override
	{
		PVOID extra = 0;
		pSender->GetConnectionExtra(dwConnID, &extra);
		((HttpAgentExtra*)extra)->recv.clear();
		return HPR_OK;
	}
	virtual EnHttpParseResult OnRequestLine(IHttpAgent* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl) override
	{
		return HPR_OK;
	}
	virtual EnHttpParseResult OnStatusLine(IHttpAgent* pSender, CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc) override
	{
		return HPR_OK;
	}
	virtual EnHttpParseResult OnHeader(IHttpAgent* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue) override
	{
		return HPR_OK;
	}
	virtual EnHttpParseResult OnHeadersComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
		IHttpServer* server = nullptr;
		CONNID remote_connid;
		HttpAgentExtra* extra = nullptr;
		{
			PVOID extra_pv = 0;
			pSender->GetConnectionExtra(dwConnID, &extra_pv);
			extra = ((HttpAgentExtra*)extra_pv);
			remote_connid = extra->connid;
			server = (IHttpServer*)extra->server->hpserver();
		}
		THeader* local_header = nullptr;
		DWORD local_hsize = 0;
		// 取所有协议头
		{
			pSender->GetAllHeaders(dwConnID, local_header, local_hsize);
			if (local_hsize == 0)
				return HPR_OK;
			local_header = new THeader[local_hsize];
			pSender->GetAllHeaders(dwConnID, local_header, local_hsize);
		}
		// 取请求方式
		server->SendResponse(
			remote_connid,
			pSender->GetStatusCode(dwConnID),
			nullptr,
			local_header,
			local_hsize);
		delete[] local_header;
		return HPR_OK;
	}
	std::string dec2hex(int i) //将int转成16进制字符串
	{
		std::stringstream ioss; //定义字符串流
		std::string s_temp; //存放转化后字符
		ioss << std::setiosflags(std::ios::uppercase) << std::hex << i; //以十六制(大写)形式输出
		//ioss << resetiosflags(ios::uppercase) << hex << i; //以十六制(小写)形式输出//取消大写的设置
		ioss >> s_temp;
		return s_temp;
	}
	virtual EnHttpParseResult OnBody(IHttpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		IHttpServer* server = nullptr;
		CONNID remote_connid;
		HttpAgentExtra* extra = nullptr;
		{
			PVOID extra_pv = 0;
			pSender->GetConnectionExtra(dwConnID, &extra_pv);
			extra = ((HttpAgentExtra*)extra_pv);
			remote_connid = extra->connid;
			server = (IHttpServer*)extra->server->hpserver();
		}
		if (extra->transfer_encoding_length != -1)
		{
			nstring length = dec2hex(iLength)+"\r\n";
			
			// 分块长度
			server->Send(
				remote_connid,
				(const BYTE*)length.c_str(),
				length.length());
			// body数据
			server->Send(
				remote_connid,
				pData,
				iLength);
		}
		else
		{
			//std::cout << "[OnBody]:" << iLength << std::endl;
			// body数据
			server->Send(
				remote_connid,
				pData,
				iLength);
		}
		
		return HPR_OK;
	}
	virtual EnHttpParseResult OnChunkHeader(IHttpAgent* pSender, CONNID dwConnID, int iLength) override
	{
		HttpAgentExtra* extra = nullptr;
		{
			PVOID extra_pv = 0;
			pSender->GetConnectionExtra(dwConnID, &extra_pv);
			extra = ((HttpAgentExtra*)extra_pv);
		}
		extra->transfer_encoding_length = iLength;

		return HPR_OK;
	}
	virtual EnHttpParseResult OnChunkComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
		IHttpServer* server = nullptr;
		HttpAgentExtra* extra = nullptr;
		{
			PVOID extra_pv = 0;
			pSender->GetConnectionExtra(dwConnID, &extra_pv);
			extra = ((HttpAgentExtra*)extra_pv);
			server = (IHttpServer*)extra->server->hpserver();
		}
		// 分块长度
		server->Send(
			extra->connid,
			(const BYTE*)"\r\n0\r\n\r\n\r\n",
			9);
		return HPR_OK;
	}
	virtual EnHttpParseResult OnMessageComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
		
		return HPR_OK;
	}
	virtual EnHttpParseResult OnUpgrade(IHttpAgent* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType) override
	{
		return HPR_OK;
	}
	virtual EnHttpParseResult OnParseError(IHttpAgent* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc) override
	{
		return HPR_OK;
	}
	virtual EnHandleResult OnWSMessageHeader(IHttpAgent* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnWSMessageBody(IHttpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnWSMessageComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnHandShake(ITcpAgent* pSender, CONNID dwConnID) override
	{
		/*成功*/
		m_map.set(dwConnID, 1, true);
		return HR_OK;
	}
	virtual EnHandleResult OnSend(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, int iLength) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnClose(ITcpAgent* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
	{
		//std::cout << "[OnClose]ConnectedCount:" << pSender->GetConnectionCount() << std::endl;
		/*失败*/
		m_map.set(dwConnID, 2, true);

		// 断开服务端连接
		{
			PVOID extra = 0;
			pSender->GetConnectionExtra(dwConnID,&extra);
			auto e = ((HttpAgentExtra*)extra);
			((IHttpServer*)e->server->hpserver())->Disconnect(e->connid);
			delete ((HttpAgentExtra*)extra);
		}
		
		m_timeout.add([&](
			timestamp create/*创建时间*/,
			uint32& timeout_msec/*过期时间[毫秒]*/,
			nstring id/*超时事件ID*/,
			ptr extra_data/*附加数据*/)->TimeOutRet {
				m_map.del(extra_data);
				return TimeOutRet::TOR_CLOSE;
			}, 3000, (ptr)dwConnID);

		return HR_OK;
	}
	virtual EnHandleResult OnShutdown(ITcpAgent* pSender) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnPrepareConnect(ITcpAgent* pSender, CONNID dwConnID, SOCKET socket) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnConnect(ITcpAgent* pSender, CONNID dwConnID) override
	{
		/*成功*/
		m_map.set(dwConnID, 1, true);

		//std::cout << "[OnConnect]ConnectedCount:" << pSender->GetConnectionCount() << std::endl;

		return HR_OK;
	}
public:
	newobj::map<uint64, int/*0=连接中 1=已连接 2=失败*/> m_map;
	newobj::timeout m_timeout;
};
uint64 newobj::network::http::agent::connect_count()
{
	return AGENT->GetConnectionCount();
}
newobj::network::http::agent::agent()
{
	m_listener = new http_agent_listener;
	m_agent = HP_Create_HttpAgent(m_listener);

}
newobj::network::http::agent::~agent()
{
	AGENT->Stop();
	AGENT->Wait();
}
bool newobj::network::http::agent::start()
{
	return AGENT->Start();
}

bool newobj::network::http::agent::stop()
{
	return AGENT->Stop();
}

bool newobj::network::http::agent::connect(const nstring& ipaddress, ushort port, int32 wait_msec, uint64& connid, uint64 server_connid, network::http::server* server)
{
	HttpAgentExtra* extra = new HttpAgentExtra;
	extra->connid = server_connid;
	extra->server = (network::http::server*)server;
	CONNID hpcid = 0;
	if (AGENT->Connect(ipaddress.c_str(), port, &hpcid, (PVOID)extra) == false)
	{
		std::cout << SYS_GetLastError() << std::endl;
		delete extra;
		return false;
	}
	
	connid = hpcid;
	timestamp start_time = time::now_msec();
	m_listener->m_map.set(hpcid, 0,true);
	while (m_listener->m_map[hpcid] == 0)
	{
		if (wait_msec != -1)
		{
			if (time::now_msec() >start_time+  wait_msec)
				break;
		}
		newobj::system::sleep_msec(10);
	}
	bool result = m_listener->m_map[hpcid] != 2;
	m_listener->m_map.del(hpcid);
	return result;
}

void newobj::network::http::agent::disconnect(uint64 connid)
{
	AGENT->Disconnect((CONNID)connid);
}

bool newobj::network::http::agent::send(network::http::proxy* proxy,uint64 agent_connid,reqpack* rp)
{
	nstring filepath = rp->url();
	nstring request_url = proxy->dst + filepath.right(filepath.length() - proxy->src.length());
	//std::cout << request_url.c_str() << std::endl;
	auto server = ((IHttpServer*)rp->server()->hpserver());
	// 取所有协议头
	// 请求协议头
	THeader* local_header = nullptr;
	// 请求协议头数量
	DWORD local_hsize = 0;
	{
		server->GetAllHeaders(rp->connid(), local_header, local_hsize);
		if (local_hsize == 0)
			return false;
		local_header = new THeader[local_hsize+ proxy->headers.size()];
		server->GetAllHeaders(rp->connid(), local_header, local_hsize);
		// 查找并替换协议头
		auto find_place_header = [&](const char* name, const char* value) {
			bool find = false;
			for (size_t i = 0; i < local_hsize; i++)
			{
				if (strcmp(local_header[i].name, name) == 0)
				{
					find = true;
					local_header[i].value = value;
					break;
				}
			}
			if (find == false)
			{
				local_header[local_hsize].name = name;
				local_header[local_hsize].value = value;
				local_hsize++;
			}
		};
		// 要求协议头
		for_iter(iter, proxy->headers)
		{
			find_place_header(iter->first.c_str(),iter->second.c_str());
		}
		//// 打印所有协议头
		//for(size_t i=0;i< local_hsize;i++)
		//{
		//	std::cout << local_header[i].name << "\t" << local_header[i].value << std::endl;
		//	std::cout << "\r\n" << std::endl;
		//}
	}
	// 取请求方式
	nstring method = server->GetMethod(rp->connid());
	//std::cout << "AgentId:" << rp->temprecv()->agent_connid <<"\t\t ConnId:"<< rp->connid() << std::endl;
	//std::cout << "Method:" << method.c_str() << std::endl;
	//std::cout << "Url:" << path.c_str() << std::endl;
	//std::cout << "\r\n" << std::endl;
	bool result = AGENT->SendRequest(
		agent_connid,
		method.c_str(),
		request_url.c_str(),
		local_header,
		local_hsize,
		(const BYTE*)rp->data()->data(),
		rp->data()->length());
	delete[] local_header;
	if (result == false)
	{
		std::cout << "agent send request failed,"<<SYS_GetLastError()<<std::endl;
	}
	return result;
}

bool newobj::network::http::agent::is_connected(uint64 connid)
{
	return AGENT->IsConnected(connid);
}
#endif