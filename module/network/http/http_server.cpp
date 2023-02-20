#include "http_server.h"
#if USE_NET_HTTP_WEBSITE
#include "http_server_lst.h"
#include "http_center.h"
#include "http_ssl.h"
#include "http_host.h"
#include "hpsocket/hpsocket.h"

#include <string.h>
#define HPSERVER ((IHttpServer*)m_server)
network::http::server::server()
{
    m_server = nullptr;
    m_listener = nullptr;
    m_https = false;
    m_port = 0;

    m_init_ssl = false;
}

network::http::server::~server()
{

}
bool network::http::server::start()
{
    //HPSERVER->SetAcceptSocketCount(10000);
    //HPSERVER->SetMaxConnectionCount(100000);
//    HPSERVER->SetWorkerThreadCount(20);
    if (HPSERVER->Start("0.0.0.0", m_port) == false)
    {
#ifndef _WIN32
        this->m_lastErrorDesc = "start failed, error code:" + nstring(SYS_GetLastErrorStr());
#else
        this->m_lastErrorDesc = "start failed, error code:" + nstring::from((uint32)SYS_GetLastError());
#endif
        return false;
    }
    if(m_agent.start() == false)
    {
        newobj::log->error("agent start failed");
    }
    return true;
}

bool newobj::network::http::server::create(bool https, ushort port)
{
    m_listener = new http_server_lst(this);
    m_listener->m_server = this;
    m_https = https;
    m_port = port;
#ifdef LIB_HPSOCKET_SSL
    if (m_https)
    {
        m_server = HP_Create_HttpsServer(m_listener);
    }
    else
    {
#endif
        m_server = HP_Create_HttpServer(m_listener);
#ifdef LIB_HPSOCKET_SSL
    }
#endif
    return true;
}

bool network::http::server::close()
{
    m_agent.stop();
    if (m_server == nullptr)
        return true;
    /*¹Ø±ÕHP HTTPSERVER*/
    if (HPSERVER->GetState() == SS_STOPPED)
        return false;
    if (HPSERVER->Stop() == false)
    {
#ifndef _WIN32
        this->m_lastErrorDesc = "stop failed, error code:" + nstring(SYS_GetLastErrorStr());
#else
        this->m_lastErrorDesc = "stop failed, error code:" + nstring::from((uint32)SYS_GetLastError());
#endif
        return false;
    }
    HPSERVER->Wait(-1);
#ifdef LIB_HPSOCKET_SSL
    /*Ïú»ÙÊÍ·Å*/
    if(m_https)
        HP_Destroy_HttpsServer(HPSERVER);
    else
#endif
        HP_Destroy_HttpServer(HPSERVER);
    delete m_listener;

    m_server = nullptr;
    m_listener = nullptr;



    return true;
}

bool network::http::server::remote(uint64 connid, nstring &ipaddress, ushort &port)
{
    int ipAddrLen = 128;
    TCHAR ipAddr[128];
    memset(ipAddr, 128, 0);
    f_ret_f(HPSERVER->GetRemoteAddress((CONNID)connid, ipAddr, ipAddrLen,port));
    ipaddress = ipAddr;
    port = port;
    return true;
}

void *network::http::server::hpserver()
{
    return m_server;
}
network::qps* newobj::network::http::server::qps()
{
    return &m_qps;
}
#endif
