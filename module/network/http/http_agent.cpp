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
	struct Request
	{
		Request()
		{
			headers = nullptr;
			headers_size = 0;
		}
		~Request()
		{
			clear();
		}
		void clear()
		{
			if (headers != nullptr)
				delete[] headers;
			headers = nullptr;
			headers_size = 0;
			path.clear();
			data.clear();
		}
		// 请求头
		THeader* headers;
		DWORD headers_size;
		// 请求路径
		nstring path;
		// 请求类型
		nstring method;
		// 请求数据
		newobj::buffer data;
	};
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

	// 请求
	Request request;
};

#define AGENT ((IHttpAgent*)get())

class http_agent_listener :public IHttpAgentListener
{
	public:
	http_agent_listener()
	{
		m_agent = nullptr;
	}
	// 通过 IHttpAgentListener 继承
	virtual EnHttpParseResult OnMessageBegin(IHttpAgent* pSender, CONNID dwConnID) override
	{
		//newobj::log->info("OnMessageBegin", "agent");

		PVOID extra = 0;
		pSender->GetConnectionExtra(dwConnID, &extra);
		((HttpAgentExtra*)extra)->recv.clear();
		((HttpAgentExtra*)extra)->transfer_encoding_length = -1;
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
		
		return HPR_OK;
	}
	virtual EnHttpParseResult OnMessageComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
		HttpAgentExtra* extra = nullptr;
		IHttpServer* server = nullptr;
		{
			PVOID extra_pv = 0;
			pSender->GetConnectionExtra(dwConnID, &extra_pv);
			extra = ((HttpAgentExtra*)extra_pv);
			server = (IHttpServer*)extra->server->hpserver();
		}
		if (extra->transfer_encoding_length != -1)
		{
			// 分块长度
			server->Send(
				extra->connid,
				(const BYTE*)"\r\n0\r\n\r\n\r\n",
				9);
		}
		
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
		newobj::log->info("OnHandShake", "agent");
		HttpAgentExtra* extra = nullptr;
		IHttpServer* server = nullptr;
		{
			PVOID extra_pv = 0;
			pSender->GetConnectionExtra(dwConnID, &extra_pv);
			extra = ((HttpAgentExtra*)extra_pv);
			server = (IHttpServer*)extra->server->hpserver();
		}
		// 取所有协议头
		// 请求协议头
		THeader* local_header = nullptr;

		//std::cout << "AgentId:" << rp->temprecv()->agent_connid <<"\t\t ConnId:"<< rp->connid() << std::endl;
		//std::cout << "Method:" << method.c_str() << std::endl;
		//std::cout << "Url:" << path.c_str() << std::endl;
		//std::cout << "\r\n" << std::endl;
		bool result = m_agent->SendRequest(
			dwConnID,
			extra->request.method.c_str(),
			extra->request.path.c_str(),
			extra->request.headers,
			extra->request.headers_size,
			(const BYTE*)extra->request.data.data(),
			extra->request.data.length());
		delete[] local_header;
		if (result == false)
		{
			std::cout << "agent send request failed," << SYS_GetLastError() << std::endl;
		}
		return HR_OK;
	}
	virtual EnHandleResult OnSend(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		newobj::log->info("OnSend", "agent");
		return HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		//newobj::log->info("OnReceive", "agent");
		return HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, int iLength) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnClose(ITcpAgent* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
	{
		newobj::log->info("OnClose", "agent");
		//std::cout << "[OnClose]ConnectedCount:" << pSender->GetConnectionCount() << std::endl;
		/*失败*/
		// 断开服务端连接
		{
			PVOID extra = 0;
			pSender->GetConnectionExtra(dwConnID,&extra);
			auto e = ((HttpAgentExtra*)extra);
			((IHttpServer*)e->server->hpserver())->Disconnect(e->connid);
			delete ((HttpAgentExtra*)extra);
		}

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
		newobj::log->info("OnConnect", "agent");
		//std::cout << "[OnConnect]ConnectedCount:" << pSender->GetConnectionCount() << std::endl;
		return HR_OK;
	}
public:
	IHttpAgent* m_agent;
};
uint64 newobj::network::http::agent::connect_count()
{
	return AGENT->GetConnectionCount();
}
bool newobj::network::http::agent::request(const nstring& ipaddress, ushort port, int32 wait_msec, uint64& connid, uint64 server_connid, network::http::server* server, reqpack* rp, network::http::proxy* proxy)
{
	HttpAgentExtra* extra = new HttpAgentExtra;
	extra->connid = server_connid;
	extra->server = server;
	CONNID hpcid = 0;

	extra->request.clear();
	extra->request.path = proxy->dst + rp->filepath().right(rp->filepath().length() - proxy->src.length());
	extra->request.data = *rp->data();
	// 构造请求内容
	{
		{
			((IHttpServer*)server->hpserver())->GetAllHeaders(rp->connid(), extra->request.headers,extra->request.headers_size);
			if (extra->request.headers_size == 0)
				return false;
			extra->request.headers = new THeader[extra->request.headers_size + proxy->headers.size()];
			((IHttpServer*)server->hpserver())->GetAllHeaders(rp->connid(), extra->request.headers, extra->request.headers_size);
			uint32 request_header_size = extra->request.headers_size;
			// 查找并替换协议头
			auto find_place_header = [&](const char* name, const char* value) {
				bool find = false;
				for (size_t i = 0; i < extra->request.headers_size; i++)
				{
					if (strcmp(extra->request.headers[i].name, name) == 0)
					{
						find = true;
						extra->request.headers[i].value = value;
						break;
					}
				}
				if (find == false)
				{
					extra->request.headers[extra->request.headers_size].name = name;
					extra->request.headers[extra->request.headers_size].value = value;
					request_header_size++;
				}
			};
			// 要求协议头
			for_iter(iter, proxy->headers)
				find_place_header(iter->first.c_str(), iter->second.c_str());

			extra->request.headers_size = request_header_size;
		}
		// 取请求方式
		extra->request.method = ((IHttpServer*)server->hpserver())->GetMethod(server_connid);
	}
	
	if (AGENT->Connect(ipaddress.c_str(), port, &hpcid, (PVOID)extra) == false)
	{
		std::cout << SYS_GetLastError() << std::endl;
		delete extra;
		return false;
	}
	connid = hpcid;
	return true;
}
void* newobj::network::http::agent::get()
{
	//if (m_ssl)
		return m_agent;
	//return m_agent_ssl;
	//return nullptr;
}
newobj::network::http::agent::agent()
{
	m_listener = new http_agent_listener;
	
	m_agent = HP_Create_HttpsAgent(m_listener);
	//m_agent_ssl = HP_Create_HttpsAgent(m_listener);
	m_listener->m_agent = (IHttpAgent*)m_agent;
} 
newobj::network::http::agent::~agent()
{
	
	AGENT->Stop();
	AGENT->Wait();
	//AGENT_SSL->Stop();
	//AGENT_SSL->Wait();
}
bool newobj::network::http::agent::start()
{
	
	//AGENT->CleanupSSLContext();
	//std::cout << AGENT->GetSSLCipherList() << std::endl;
	// 
	
	AGENT->CleanupSSLContext();
	//AGENT->SetSSLCipherList("DEFAULT:!aNULL:!eNULL:!SSLv3");
	//AGEN
	if (AGENT->SetupSSLContext() == false)
	{
		std::cout << "SetupSSLContextFailed, " << nstring::from((uint32)SYS_GetLastError()).c_str() << std::endl;
		return false;
	}
	

	return AGENT->Start();//&& AGENT_SSL->Start();
}

bool newobj::network::http::agent::stop()
{
	return AGENT->Stop();//&& AGENT_SSL->Stop();
}

bool newobj::network::http::agent::connect(const nstring& ipaddress, ushort port, int32 wait_msec, uint64& connid, uint64 server_connid, network::http::server* server)
{
	/*auto ba = time::now_msec();
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

	std::cout << "Connect:" << time::now_msec() - ba << std::endl;
	ba = time::now_msec();
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
	std::cout << "ConnectEnd:" <<time::now_msec()- ba << std::endl;
	return result;*/
	return false;
}

void newobj::network::http::agent::disconnect(uint64 connid)
{
	AGENT->Disconnect((CONNID)connid,TRUE);
	//AGENT_SSL->Disconnect((CONNID)connid);
}

//bool newobj::network::http::agent::send(network::http::proxy* proxy,uint64 agent_connid,reqpack* rp)
//{
//	return true;
//}

bool newobj::network::http::agent::is_connected(uint64 connid)
{
	return AGENT->IsConnected(connid);
}
#endif