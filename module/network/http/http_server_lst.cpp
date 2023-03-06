#include "http_server_lst.h"
#if USE_NET_HTTP_WEBSITE
#include "util/forstring.h"
#include "http_server.h"
#include "hpsocket/hpsocket.h"
#include <algorithm>
#include "public/exception.h"

#include "http_define.h"
#include "http_reqpack.h"
#include "http_router.h"
#include "http_center.h"
#include "http_website.h"
#include "http_agent.h"
#include "http_util.h"


        
newobj::network::http::http_server_lst::http_server_lst(server* server)
{
	
}

newobj::network::http::http_server_lst::~http_server_lst()
{
	
}

EnHandleResult newobj::network::http::http_server_lst::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
{
	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
{
	//std::cout << "[OnAccept] HttpServerConnect:" << pSender->GetConnectionCount() << std::endl;
#if BARE_HP == 0
	pSender->SetConnectionExtra(dwConnID, (PVOID)m_server->m_extra_queue->create());
#endif
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnAccept ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnHandShake(ITcpServer* pSender, CONNID dwConnID)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnHandShake ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength)
{
	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnReceive ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnSend ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	m_server->qps()->response(iLength,true);
	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnShutdown(ITcpServer* pSender)
{
	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnClose ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif
#if BARE_HP == 0
	PVOID extra = 0;
	if (pSender->GetConnectionExtra(dwConnID, &extra))
	{
		if (extra != 0)
		{
			temp_recv* tr = (temp_recv*)extra;
			if (tr->agent_connid != 0)
			{
                m_server->agent()->disconnect(tr->agent_ssl,tr->agent_connid);
			}
			m_server->m_extra_queue->destory((temp_recv*)extra);
		}
			
	}
#endif
	return HR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnMessageBegin(IHttpServer* pSender, CONNID dwConnID)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnMessageBegin ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif
#if BARE_HP == 0
	PVOID extra = 0;
	if (pSender->GetConnectionExtra(dwConnID, &extra))
	{
		if (extra != 0)
        {
			((temp_recv*)extra)->data = m_server->m_extra_data_queue->create();
		}
	}
#endif
	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnRequestLine(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl)
{
//#if HTTP_SERVER_DEBUG_PRINT == 1
//     newobj::log->info("OnRequestLine ("+nstring::from((uint64)dwConnID)+")","http_server");
//#endif

#if BARE_HP == 0
	PVOID extra = 0;
	if (pSender->GetConnectionExtra(dwConnID, &extra))
	{
		if (extra != 0)
		{
			((temp_recv*)extra)->url = lpszUrl;
		}
	}
#endif
	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnStatusLine(IHttpServer* pSender, CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc)
{
//#if HTTP_SERVER_DEBUG_PRINT == 1
//     newobj::log->info("OnStatusLine ("+nstring::from((uint64)dwConnID)+")","http_server");
//#endif

	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue)
{
//#if HTTP_SERVER_DEBUG_PRINT == 1
//     newobj::log->info("OnHeader ("+nstring::from((uint64)dwConnID)+")","http_server");
//#endif

    if(strcmp(lpszName,"Host") == 0){
        PVOID extra = 0;
        if (pSender->GetConnectionExtra(dwConnID, &extra))
        {
            if (extra != 0)
            {
                ((temp_recv*)extra)->host = lpszValue;
            }
        }
   
    }
	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID)
{
	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnChunkHeader(IHttpServer* pSender, CONNID dwConnID, int iLength)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnChuckHeader ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	return HPR_OK;
}
EnHttpParseResult newobj::network::http::http_server_lst::OnChunkComplete(IHttpServer* pSender, CONNID dwConnID)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnChunkComplete ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnUpgrade ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnBody ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

#if BARE_HP == 0
	PVOID extra = 0;
	if (pSender->GetConnectionExtra(dwConnID, &extra))
	{
		if (extra != 0)
            ((temp_recv*)extra)->data->append((char*)pData, iLength);
	}
#endif
	return HPR_OK;
}

EnHttpParseResult newobj::network::http::http_server_lst::OnMessageComplete(IHttpServer* pSender, CONNID dwConnID)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnMessageComplete ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif
#if BARE_HP == 1
	pSender->SendResponse(dwConnID, 200, "OK", nullptr, 0, (const BYTE*)"ONLY_HP_OK", 3);
	return HPR_OK;
#endif
    reqpack* rp = nullptr;
    {
        newobj::buffer* recv_data = nullptr;
		temp_recv* tr = nullptr;
        PVOID extra = 0;
        if (pSender->GetConnectionExtra((CONNID)dwConnID, &extra))
        {
			if (extra != 0)
				tr = (temp_recv*)extra;
        }
		
		m_server->qps()->request(tr->data->length()); 

		rp = reqpack::create();
        rp->init(tr->url,tr->host,tr->data,(uint64)dwConnID,m_server);
		tr->data = nullptr;
    }
    nstring size_name;
    {
        double size_ld = 0;
        if(rp->data() != nullptr){
            size_ld = rp->data()->length();
        }
        size_name = newobj::network::tools::size_name(size_ld,2);
        /*if(nstring(pSender->GetMethod(dwConnID)) == "GET" && rp->data()->length() != 0){
            newobj::log->warn("GET have body:"+rp->data()->to_string(),"http_server");
        }*/
    }
#if HTTP_SERVER_PRINT == 1
	// LOG
	nstring logstr;
	{
		logstr.append("[recv   ]\t");
		logstr.append(rp->remote());
		logstr.append("\t(");
		logstr.append(nstring::from(rp->connid()));
		logstr.append(")\t\t");
		logstr.append(nstring(pSender->GetMethod(dwConnID)));
		logstr.append("\t");
		logstr.append(size_name);
		logstr.append("\t");
		logstr.append(rp->host());
		logstr.append(rp->url());
	}
#endif
	auto website = m_server->center()->website(rp->host());
	if (website == nullptr)
	{
	    pSender->SendResponse(dwConnID, 404, "Not Found", nullptr, 0, (const BYTE*)"No such site",12);
#if HTTP_SERVER_PRINT == 1
        logstr.append(" not website("+rp->host()+"):");
        newobj::log->error(logstr,"http_server");
#endif
		reqpack::destory(rp);
		return HPR_OK;
	}
#if HTTP_SERVER_PRINT == 1
    newobj::log->info(logstr,"http_server");
#endif
	rp->website(website);
#if 0
	rp->request();
	rp->response();
	reqpack::destory(rp);
	pSender->SendResponse(dwConnID, 200, "OK", nullptr, 0, (const BYTE*)"V24", 3);
	return HPR_OK;

#endif
	auto recvd_callback = website->router()->m_callback_recved;
	newobj::buffer end_data;
	if (recvd_callback != nullptr)
	{
		recvd_callback(*rp->data(), &end_data);
		if (end_data.length() != 0)
			*rp->data() = end_data;
	}
        


    website->router()->push(rp);
#if 0
    nstring filepath = pSender->GetUrlField((CONNID)dwConnID, EnHttpUrlField::HUF_PATH);

    filepath = "/home/nianhua/project/mysql_demo/www"+filepath;
    pSender->SendLocalFile(dwConnID,filepath.c_str(),HSC_OK);
#endif

	
	return HPR_OK;

}

EnHttpParseResult newobj::network::http::http_server_lst::OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc)
{
#if HTTP_SERVER_DEBUG_PRINT == 1
     newobj::log->info("OnParseError ("+nstring::from((uint64)dwConnID)+")","http_server");
#endif

	PVOID extra = 0;
	if (pSender->GetConnectionExtra(dwConnID, &extra))
	{
		if (extra != 0)
		{
			delete ((temp_recv*)extra)->data;
			((temp_recv*)extra)->data = nullptr;
		}
	}
	pSender->SetConnectionExtra(dwConnID,0);
	return HPR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnWSMessageHeader(IHttpServer* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen)
{
	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	return HR_OK;
}

EnHandleResult newobj::network::http::http_server_lst::OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID)
{
	return HR_OK;
}
#endif
