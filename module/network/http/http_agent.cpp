#include "http_agent.h"
#if USE_NET_HTTP_WEBSITE
#include "http_reqpack.h"
#include "http_define.h"
#include "hpsocket/hpsocket.h"
//#include "hpsocket/windows/include/ssl/HPSocket-SSL.h"
#include "util/system.h"
#include "util/time.h"
#include "util/timeout.h"
#include "util/map.hpp"
#include "http_server.h"
#include "http_request.h"
#include "http_util.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
/*附加数据*/
class http_agent_extra
{
public:
	// 请求体
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
		// 请求头
		std::vector<Kv> headers;
		// 请求路径
		nstring path;
		// 请求类型
		nstring method;
		// 请求数据
		newobj::buffer data;
		// HOST:PORT
		nstring url_host;

	};
	// 接收
	struct receive {
		receive() {
			clear();
		}
		void clear() {
			data.clear();
			transfer_encoding_length = -1;
		}
		// 回复数据
		newobj::buffer data;
		// check 类型数据头内容 默认=-1 即非该类型回复。
		int32 transfer_encoding_length;
	};
	http_agent_extra()
	{
		connid = 0;
		server = nullptr;
		agent = nullptr;
	}

	void clear()
	{
		connid = 0;
		server = nullptr;
		req.clear();
		recv.clear();
		agent = nullptr;
	}
	// server 客户连接ID
	uint64 connid;
	// 绑定server
	network::http::server* server;
	// 请求
	request req;
	// 接收
	receive recv;
	// 代理
	IHttpAgent* agent;
};
// 获取附加数据指针
inline http_agent_extra* __get_extra_to__(ITcpAgent* client, CONNID dwConnID){
    PVOID extra = 0;
    client->GetConnectionExtra(dwConnID, &extra);
    return (http_agent_extra*)extra;
}
#define GET_EXTRA __get_extra_to__(pSender,dwConnID)
#define SACONNID nstring("(S:"+nstring::from(GET_EXTRA->connid)+"|A["+nstring::from((uint64)pSender)+"]:"+nstring::from((uint64)dwConnID)+")")
class http_agent_listener :public IHttpAgentListener
{
	public:
	http_agent_listener()
	{
	}
	// 通过 IHttpAgentListener 继承
	virtual EnHttpParseResult OnMessageBegin(IHttpAgent* pSender, CONNID dwConnID) override
	{
	    #if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnMessageBegin "+SACONNID,"http_agent ");
	    #endif
        auto extra = GET_EXTRA;
		extra->recv.clear();
		return HPR_OK;
	}
	virtual EnHttpParseResult OnRequestLine(IHttpAgent* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl) override
	{

//	    #if HTTP_AGENT_DEBUG_PRINT == 1
//	    newobj::log->info("OnRequestLine "+SACONNID,"http_agent ");
//	    #endif

		return HPR_OK;
	}
	virtual EnHttpParseResult OnStatusLine(IHttpAgent* pSender, CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc) override
	{
//	    #if HTTP_AGENT_DEBUG_PRINT == 1
//	    newobj::log->info("OnStatusLine "+SACONNID,"http_agent ");
//	    #endif

		return HPR_OK;
	}
	virtual EnHttpParseResult OnHeader(IHttpAgent* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue) override
	{
//	    #if HTTP_AGENT_DEBUG_PRINT == 1
//	    newobj::log->info("OnHeader "+SACONNID,"http_agent ");
//	    #endif

		return HPR_OK;
	}
	virtual EnHttpParseResult OnHeadersComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
	    #if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnHeadersComplete "+SACONNID,"http_agent ");
	    #endif
		http_agent_extra* extra = GET_EXTRA;
		IHttpServer* server = (IHttpServer*)extra->server->hpserver();
		THeader* local_header = nullptr;
		DWORD local_hsize = 0;
		// 取所有协议头
        pSender->GetAllHeaders(dwConnID, local_header, local_hsize);
        if (local_hsize == 0)
            return HPR_OK;
        local_header = new THeader[local_hsize];
        pSender->GetAllHeaders(dwConnID, local_header, local_hsize);
		// 取请求方式
		server->SendResponse(
			(CONNID)extra->connid,
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
	    #if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnBody "+SACONNID,"http_agent ");
	    #endif
        http_agent_extra* extra = GET_EXTRA;
		IHttpServer* server = (IHttpServer*)extra->server->hpserver();
		
		
		// Chunk 数据
		if(extra->recv.transfer_encoding_length != -1){
		    extra->recv.data.append((const char*)pData,iLength);
		}else{
		    server->Send(
                     (CONNID)extra->connid,
                     pData,
                     iLength);
		}
		
        return HPR_OK;
	}
	virtual EnHttpParseResult OnChunkHeader(IHttpAgent* pSender, CONNID dwConnID, int iLength) override
	{
	    #if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnChunkHeader "+nstring::from(iLength)+SACONNID,"http_agent ");
	    #endif
        http_agent_extra* extra = GET_EXTRA;
        //if(extra->recv.transfer_encoding_length == -1 ){
        //    extra->recv.transfer_encoding_length = iLength;
        //}else{
            extra->recv.transfer_encoding_length += iLength;
        //}
        
        /*
        // Chunk 头
		if(extra->recv.transfer_encoding_length > 0){
		    IHttpServer* server = (IHttpServer*)extra->server->hpserver();
		    nstring length = dec2hex(extra->recv.transfer_encoding_length)+"\r\n";   
		    // 分块长度
		    server->Send(
				extra->connid,
				(const BYTE*)length.c_str(),
				length.length());
		}
        */
        
		return HPR_OK;
	}
	virtual EnHttpParseResult OnChunkComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{
	    #if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnChunkComplete:"+SACONNID,"http_agent ");
	    #endif
		return HPR_OK;
	}
	virtual EnHttpParseResult OnMessageComplete(IHttpAgent* pSender, CONNID dwConnID) override
	{



	    http_agent_extra* extra = GET_EXTRA;

		#if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnMessageComplete:"+SACONNID,"http_agent ");
	    #endif



		if(extra->recv.transfer_encoding_length >= 0){
		    IHttpServer* server = (IHttpServer*)extra->server->hpserver();
		    
		    
		    nstring length = dec2hex(extra->recv.transfer_encoding_length+1)+"\r\n";   
		    // 分块长度
		    server->Send(
				(CONNID)extra->connid,
				(const BYTE*)length.c_str(),
				length.length());
			// 数据包
		    server->Send(
				(CONNID)extra->connid,
                     (const BYTE*)extra->recv.data.data(),
                     extra->recv.data.length()
                     );
            // 结尾
		    server->Send(
				(CONNID)extra->connid,
                     (const BYTE*)"\r\n0\r\n\r\n",
                     9);
		}
		return HPR_OK;
	}
	virtual EnHttpParseResult OnUpgrade(IHttpAgent* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType) override
	{
		#if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnUpgrade:"+SACONNID,"http_agent ");
	    #endif

		return HPR_OK;
	}
	virtual EnHttpParseResult OnParseError(IHttpAgent* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc) override
	{
#if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnParsrError:"+SACONNID,"http_agent ");
#endif

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
#if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnHandShake:"+SACONNID,"http_agent ");
#endif

		http_agent_extra* extra = GET_EXTRA;
		// 发送请求
        //newobj::log->info("[request]\t\t\t(S:"+nstring::from(extra->connid)+"|A:"+nstring::from((uint64)dwConnID)+")\t" +extra->req.method +"\t"+newobj::network::tools::size_name((double)extra->req.data.length(),2)+"\t"+extra->req.url_host,"http_agent ");
        THeader* local_header = new THeader[extra->req.headers.size()];
        size_t i= 0;
        for_iter(iter,extra->req.headers){
            local_header[i].name = iter->name.c_str();
            local_header[i].value = iter->value.c_str();
            i++;
        }
       // std::cout<<"RequestLength:"<<extra->req.data.length()<<std::endl; 
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
#if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnSend:"+SACONNID,"http_agent ");
#endif

		return HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
#if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnReceive:"+SACONNID,"http_agent ");
#endif

		//newobj::log->info("OnReceive", "agent");
		return HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, int iLength) override
	{
		return HR_OK;
	}
	virtual EnHandleResult OnClose(ITcpAgent* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
	{
	    #if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnClose "+SACONNID,"http_agent ");
	    #endif
	    
        http_agent_extra* extra = GET_EXTRA;
        //IHttpServer* server = (IHttpServer*)extra->server->hpserver();
        //server->Disconnect(extra->connid);
		m_agent->m_extra_queue->destory(extra);
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
#if HTTP_AGENT_DEBUG_PRINT == 1
	    newobj::log->info("OnConnect:"+SACONNID,"http_agent ");
#endif

		return HR_OK;
	}
public:
	network::http::agent* m_agent;

};
bool newobj::network::http::agent::request(int32 wait_msec,reqpack* rp, network::http::proxy* proxy)
{
	
	http_agent_extra* extra = m_extra_queue->create();

    extra->connid = rp->connid();
	extra->server = rp->server();

    IHttpAgent* agent = nullptr;
    if(proxy->ssl)
        agent = (IHttpAgent*)m_agent_ssl;
    else
        agent = (IHttpAgent*)m_agent;

    extra->agent = agent;
	extra->req.clear();
    // 拼接地址
    {
		try
		{

			size_t right_length = rp->url().length() - newobj::file::pre_dir(proxy->src_str).length();
			nstring pjp;
			pjp.append(rp->url().c_str() + rp->url().length() - right_length, right_length);
			extra->req.path.append(proxy->dst);

			if (
				(proxy->dst.empty() || proxy->dst[proxy->dst.length()-1] != '/') 
				&& pjp[0] != '/') {
				extra->req.path.append('/');
			}
			extra->req.path.append(pjp);
		}
		catch (const std::exception& e)
		{
			newobj::log->fatal(e.what());
		}
        
    }
	extra->req.data = *rp->data();
	extra->req.url_host;
	{
		extra->req.url_host.append(proxy->remote_ipaddress);
		extra->req.url_host.append(":");
		extra->req.url_host.append(nstring::from(proxy->remote_port));
		extra->req.url_host.append(extra->req.path);
	}
	// 构造请求内容
	{
        THeader* header = nullptr;
        DWORD header_size = 0;
        ((IHttpServer*)rp->server()->hpserver())->GetAllHeaders((CONNID)rp->connid(), header,header_size);
        if (header_size == 0){
#if HTTP_AGENT_PRINT == 1
            newobj::log->error("not found header,"+extra->req.path,"http_agent ");
#endif
			m_extra_queue->destory(extra);
            return false;
        }
        header = new THeader[header_size];
        ((IHttpServer*)rp->server()->hpserver())->GetAllHeaders((CONNID)rp->connid(), header,header_size);
        // 浏览器真实IP
        nstring ipaddress;
        ushort port = 0;
        rp->server()->remote(rp->connid(),ipaddress,port);
        // 请求协议头
        for(size_t i=0;i<header_size;i++){
            Kv kv;
            kv.name = header[i].name;
            kv.value = header[i].value;
            // 要求协议头
            for_iter(iter, proxy->headers){
                if(kv.name == iter->first){
                    kv.value = iter->second;
                    break;
                }
            }
            extra->req.headers.push_back(kv);
        }
        for_iter(iter,extra->req.headers){
            if(iter->name == "X-Real-IP"){
                if(iter->value == "{client_ipaddress}"){
                    iter->value = ipaddress;
                }
            }
            else if(iter->name == "Host"){
                if(iter->value == "{host}"){
                    iter->value  = proxy->remote_ipaddress;
                }
            }
        }

        delete[] header;
        // 取请求方式
		extra->req.method = ((IHttpServer*)rp->server()->hpserver())->GetMethod((CONNID)rp->connid());
	}
#if 0
    std::cout<<"======================HEADER=========================="<<std::endl;
    for(size_t i=0;i<extra->req.headers.size();i++){
        std::cout<<extra->req.headers[i].name.c_str()<<":\t"<<extra->req.headers[i].value.c_str()<<std::endl;
    }
#endif
    CONNID hpcid = 0;
	if (extra->agent->Connect(proxy->remote_ipaddress.c_str(), proxy->remote_port, &hpcid, (PVOID)extra) == false)
	{
#if HTTP_AGENT_PRINT == 1
        newobj::log->error("connect failed.\t"+extra->req.path,"http_agent ");
#endif
		m_extra_queue->destory(extra);
		return false;
	}
    // 临时附加数据
    {
        PVOID exts = 0;
        if (((IHttpServer*)rp->server()->hpserver())->GetConnectionExtra((CONNID)rp->connid(), &exts) == false){
            agent->Disconnect((CONNID)hpcid);
#if HTTP_AGENT_PRINT == 1
            newobj::log->error("get server extra failed,"+extra->req.path,"http_agent ");
#endif
            return false;
        }
        ((temp_recv*)exts)->agent_connid = hpcid;
        ((temp_recv*)exts)->agent_ssl = proxy->ssl;
    }

	return true;
}
void* newobj::network::http::agent::get()
{
    return m_agent;
}
newobj::network::http::agent::agent()
{
	m_agent = nullptr;
	m_agent_ssl = nullptr;
	m_extra_queue = new newobj::object_pool<http_agent_extra>("http_agent_extra", 10000, 60);
	//
	m_listener = new http_agent_listener;
	m_listener->m_agent = this;
	m_agent_ssl = HP_Create_HttpsAgent(m_listener);
	m_agent = HP_Create_HttpAgent(m_listener);
} 
newobj::network::http::agent::~agent()
{
    stop();
    HP_Destroy_HttpAgent((IHttpAgent*)m_agent);
    HP_Destroy_HttpsAgent((IHttpAgent*)m_agent_ssl);
	delete m_extra_queue;
}
void newobj::network::http::agent::stop()
{
 	((IHttpAgent*)m_agent)->Stop();
	((IHttpAgent*)m_agent_ssl)->Stop();
	((IHttpAgent*)m_agent_ssl)->Wait();
	((IHttpAgent*)m_agent_ssl)->Wait();
}
bool newobj::network::http::agent::start()
{
	//return true;
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
