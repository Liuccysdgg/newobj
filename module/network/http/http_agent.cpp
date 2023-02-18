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
#include "http_request.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
/*��������*/
struct http_agent_extra
{
    // ������
	struct request
	{
		request()
		{
            clear();
		}
		~request()
		{
			clear();
		}
		void clear()
		{
			headers.clear();
            path.clear();
			data.clear();
		}
		// ����ͷ
        std::map<nstring,nstring> headers;
		// ����·��
		nstring path;
		// ��������
		nstring method;
		// ��������
		newobj::buffer data;
        // HOST:PORT
        nstring url_hout;

	};
    // ����
    struct receive{
        receive(){
            clear();
        }
        void clear(){
            data.clear();
            transfer_encoding_length = -1;
        }
        // �ظ�����
        newobj::buffer data;
        // check ��������ͷ���� Ĭ��=-1 ���Ǹ����ͻظ���
        int32 transfer_encoding_length;
    };
	http_agent_extra()
	{
		connid = 0;
		server = nullptr;
        agent = nullptr;
	}
    // server �ͻ�����ID
    uint64 connid;
    // ��server
    network::http::server* server;
	// ����
	request req;
    // ����
    receive recv;
    // ����
    IHttpAgent* agent;
};
// ��ȡ��������ָ��
inline http_agent_extra* __get_extra_to__(ITcpAgent* client, CONNID dwConnID){
    PVOID extra = 0;
    client->GetConnectionExtra(dwConnID, &extra);
    return (http_agent_extra*)extra;
}
#define GET_EXTRA __get_extra_to__(pSender,dwConnID)
class http_agent_listener :public IHttpAgentListener
{
	public:
	http_agent_listener()
	{
	}
	// ͨ�� IHttpAgentListener �̳�
	virtual EnHttpParseResult OnMessageBegin(IHttpAgent* pSender, CONNID dwConnID) override
	{
        auto extra = GET_EXTRA;
		extra->recv.clear();
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
		http_agent_extra* extra = GET_EXTRA;
		IHttpServer* server = (IHttpServer*)extra->server->hpserver();
		THeader* local_header = nullptr;
		DWORD local_hsize = 0;
		// ȡ����Э��ͷ
        pSender->GetAllHeaders(dwConnID, local_header, local_hsize);
        if (local_hsize == 0)
            return HPR_OK;
        local_header = new THeader[local_hsize];
        pSender->GetAllHeaders(dwConnID, local_header, local_hsize);
		// ȡ����ʽ
		server->SendResponse(
			extra->connid,
			pSender->GetStatusCode(dwConnID),
			nullptr,
			local_header,
			local_hsize);
		delete[] local_header;
		return HPR_OK;
	}
	std::string dec2hex(int i) //��intת��16�����ַ���
	{
		std::stringstream ioss; //�����ַ�����
		std::string s_temp; //���ת�����ַ�
		ioss << std::setiosflags(std::ios::uppercase) << std::hex << i; //��ʮ����(��д)��ʽ���
		//ioss << resetiosflags(ios::uppercase) << hex << i; //��ʮ����(Сд)��ʽ���//ȡ����д������
		ioss >> s_temp;
		return s_temp;
	}
	virtual EnHttpParseResult OnBody(IHttpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
        http_agent_extra* extra = GET_EXTRA;
		IHttpServer* server = (IHttpServer*)extra->server->hpserver();
        server->Send(
                     extra->connid,
                     pData,
                     iLength);
        return HPR_OK;
	}
	virtual EnHttpParseResult OnChunkHeader(IHttpAgent* pSender, CONNID dwConnID, int iLength) override
	{
        http_agent_extra* extra = GET_EXTRA;
		IHttpServer* server = (IHttpServer*)extra->server->hpserver();
		nstring length = dec2hex(iLength)+"\r\n";
			// �ֿ鳤��
		server->Send(
				extra->connid,
				(const BYTE*)length.c_str(),
				length.length());
		GET_EXTRA->recv.transfer_encoding_length = iLength;
		return HPR_OK;
	}
	virtual EnHttpParseResult OnChunkComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
        http_agent_extra* extra = GET_EXTRA;
		IHttpServer* server = (IHttpServer*)extra->server->hpserver();
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
		http_agent_extra* extra = GET_EXTRA;
		// ��������
        newobj::log->info("[request] "+extra->req.method +"\t"+extra->req.url_hout,"http_agent");
        THeader* local_header = new THeader[extra->req.headers.size()];
        size_t i= 0;
        for_iter(iter,extra->req.headers){
            local_header[i].name = iter->first.c_str();
            local_header[i].value = iter->second.c_str();
            i++;
        }

		bool result = extra->agent->SendRequest(
			dwConnID,
			extra->req.method.c_str(),
			extra->req.path.c_str(),
			local_header,
			extra->req.headers.size(),
			(const BYTE*)extra->req.data.data(),
			extra->req.data.length());
		delete[] local_header;
		return HR_OK;
	}
	virtual EnHandleResult OnSend(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
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
        http_agent_extra* extra = GET_EXTRA;
		IHttpServer* server = (IHttpServer*)extra->server->hpserver();
        server->Disconnect(extra->connid);
		delete extra;
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
		return HR_OK;
	}
};
bool newobj::network::http::agent::request(int32 wait_msec,reqpack* rp, network::http::proxy* proxy)
{
	http_agent_extra* extra = new http_agent_extra;
	extra->connid = rp->connid();
	extra->server = rp->server();
    if(proxy->ssl)
        extra->agent = (IHttpAgent*)m_agent_ssl;
    else
        extra->agent = (IHttpAgent*)m_agent;

    temp_recv *srv_extra = nullptr;
    //Server Extra
    {
        // ��ʱ��������
        {
            PVOID exts = 0;
            if (((IHttpServer*)rp->server()->hpserver())->GetConnectionExtra(rp->connid(), &exts) == false)
                return true;
            srv_extra = (temp_recv*)exts;
            srv_extra->agent = (void*)extra->agent;
        }
    }
    // Agent ConnID
	CONNID hpcid = 0;

	extra->req.clear();
    // ƴ�ӵ�ַ
    {
        nstring pjp = rp->url().right(rp->url().length() - proxy->src.length());
        extra->req.path.append(proxy->dst);
        if(proxy->dst.right(1) != "/" && pjp.left(1) != "/"){
            extra->req.path.append("/");
        }
        extra->req.path.append(pjp);
    }
	extra->req.data = *rp->data();
    extra->req.url_hout = proxy->remote_ipaddress+":"+nstring::from(proxy->remote_port)+extra->req.path;
	// ������������
	{
        THeader* header = nullptr;
        DWORD header_size = 0;
        ((IHttpServer*)rp->server()->hpserver())->GetAllHeaders(rp->connid(), header,header_size);
        if (header_size == 0)
            return false;
        header = new THeader[header_size];
        ((IHttpServer*)rp->server()->hpserver())->GetAllHeaders(rp->connid(), header,header_size);
        // �������ʵIP
        nstring ipaddress;
        ushort port = 0;
        rp->server()->remote(rp->connid(),ipaddress,port);
        // ����Э��ͷ
        for(size_t i=0;i<header_size;i++)
            extra->req.headers[header[i].name] = header[i].value;
        // Ҫ��Э��ͷ
        for_iter(iter, proxy->headers){
            if(iter->first == "X-Real-IP"){
                if(iter->second == "{client_ipaddress}"){
                    extra->req.headers[iter->first] = ipaddress;
                    continue;
                }
            }
            extra->req.headers[iter->first] = iter->second;
        }
        delete[] header;
        // ȡ����ʽ
		extra->req.method = ((IHttpServer*)rp->server()->hpserver())->GetMethod(rp->connid());
	}
//    for(size_t i=0;i<extra->req.headers_size;i++){
//        std::cout<<extra->req.headers[i].name<<":\t"<<extra->req.headers[i].value<<std::endl;
//    }
	if (extra->agent->Connect(proxy->remote_ipaddress.c_str(), proxy->remote_port, &hpcid, (PVOID)extra) == false)
	{
        newobj::log->error("connect failed.\t"+extra->req.path,"http_agent");
		delete extra;
		return false;
	}
	srv_extra->agent_connid = hpcid;
	return true;
}
void* newobj::network::http::agent::get()
{
    return m_agent;
}
newobj::network::http::agent::agent()
{
	m_listener = new http_agent_listener;
	m_agent_ssl = HP_Create_HttpsAgent(m_listener);
	m_agent = HP_Create_HttpAgent(m_listener);
} 
newobj::network::http::agent::~agent()
{
	((IHttpAgent*)m_agent)->Stop();
	((IHttpAgent*)m_agent_ssl)->Stop();
	((IHttpAgent*)m_agent_ssl)->Wait();
	((IHttpAgent*)m_agent_ssl)->Wait();
}
bool newobj::network::http::agent::start()
{
	
	((IHttpAgent*)m_agent_ssl)->CleanupSSLContext();
	if (((IHttpAgent*)m_agent_ssl)->SetupSSLContext() == false)
	{
		m_lastErrorDesc =  "SetupSSLContextFailed, " + nstring::from((uint32)SYS_GetLastError());
		return false;
	}
    if(((IHttpAgent*)m_agent_ssl)->Start() == false){
        m_lastErrorDesc = "SSLHttpAgent start failed.";
        return false;
    }
    if(((IHttpAgent*)m_agent)->Start() == false){
        m_lastErrorDesc = "HttpAgent start failed.";
        ((IHttpAgent*)m_agent_ssl)->Stop();
        ((IHttpAgent*)m_agent_ssl)->Wait();
        return false;

    }
    return true;

}
void newobj::network::http::agent::disconnect(bool ssl,uint64 connid)
{
	if(ssl)
        ((IHttpAgent*)m_agent_ssl)->Disconnect((CONNID)connid,TRUE);
    else
        ((IHttpAgent*)m_agent)->Disconnect((CONNID)connid,TRUE);
}
#endif
