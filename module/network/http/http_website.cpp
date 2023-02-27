#include "http_website.h"
#if USE_NET_HTTP_UTIL
#include "http_host.h"
#include "http_ssl.h"
#include "http_center.h"
#include "http_router.h"
#include "http_util.h"
#include "http_session.h"
#include "network/http/http_util.h"
#include "http_agent.h"
newobj::network::http::website::website()
{
    m_https = false;
    m_session = new network::http::session_mgr;
    m_router = new network::http::router;
   // m_agent = new network::http::agent;
    
}

newobj::network::http::website::~website()
{
    //delete m_agent;
    delete m_session;
    delete m_router;
}

bool newobj::network::http::website::start(const newobj::json& config)
{
    m_session->center(center());
    m_router->center(center());

    auto find_ssl = [&](const nstring& host)->newobj::json {
        newobj::json ssl_config = this->center()->config()["ssl"];
        return ssl_config[host];
    };
    
    if (config["host"].size() == 0)
    {
        m_lastErrorDesc = "Host name needs to be set";
        return false;
    }
    
    for (size_t i = 0; i < config["host"].size(); i++)
    {
        auto hostpoint = new network::http::host;
        newobj::json host_config = config["host"][i];
        nstring host = host_config["host"].to<nstring>();
        ushort port = 0;
        {
            nstring http_type;
            nstring ipaddress;
            nstring url_fid;
            newobj::network::tools::parse_url(host,http_type, host, ipaddress,port, url_fid);
        }
#ifdef LIB_HPSOCKET_SSL
        network::http::ssl* ssl = nullptr;
        if (host_config["ssl"].to<bool>())
        {
            m_https = true;
            newobj::json ssl_config = find_ssl(host);

            if (ssl_config["enable"].to<bool>() == false)
                newobj::log->warn("The certificate is not open or does not exist, Host:"+ host);
            else
            {
                ssl = new network::http::ssl(
                    center()->server(port),
                    (network::http::ssl_verify_type)host_config["ssl_type"].to<int>(),
                    ssl_config["pem_cert"].to<nstring>(),
                    ssl_config["pem_key"].to<nstring>(),
                    ssl_config["pem_password"].to<nstring>());
                /*注册SSL*/
                if (ssl->regist() == false)
                {
                    newobj::log->warn(ssl->last_error());
                    delete ssl;
                    ssl = nullptr;
                }
                else
                {
                    /*绑定SSL*/
                    if (ssl->bind(host) == false)
                    {
                        newobj::log->warn(ssl->last_error());
                        delete ssl;
                        ssl = nullptr;
                    }
                }
            }
        }

        hostpoint->init(host, port,ssl);
#else
        hostpoint->init(host,port,nullptr);
#endif
        m_hosts.push_back(hostpoint);
    }

    if (m_session->start() == false)
    {
        m_lastErrorDesc = m_session->last_error();
        return false;
    }
    if (m_router->start(config["router"]) == false)
    {
        m_session->close();
        m_lastErrorDesc = m_router->last_error();
        return false;
    }
    /*代理*/
    {
//        if (m_agent->start())
 //       {
            for (size_t i = 0; i < config["proxy"].size(); i++)
            {
                newobj::json proxy_cj = config["proxy"][i];
                network::http::proxy* proxy = new network::http::proxy;
                proxy->src_str = proxy_cj["src"].to<nstring>();
                proxy->src_express = std::regex(proxy->src_str.c_str());
                proxy->dst = proxy_cj["dst"].to<nstring>();

                if(proxy->dst == "/"){
                    proxy->dst.clear();
                }
                // 解析地址
                {
                    proxy->remote_url = proxy_cj["remote"].to<nstring>();
                    nstring http_type;
                    nstring http_host;
                    nstring http_ipaddress;
                    ushort http_port;
                    nstring http_urlfield;
                    newobj::network::tools::parse_url(proxy->remote_url, http_type, http_host, http_ipaddress, http_port, http_urlfield);
                    proxy->remote_ipaddress = http_host;
                    proxy->remote_port = http_port;
                    proxy->ssl = http_type=="https://";
                }
                proxy->host = proxy_cj["host"].to<nstring>();
                // 协议头
                for (size_t h = 0; h < proxy_cj["headers"].size(); h++)
                    proxy->headers[proxy_cj["headers"][h]["key"].to<nstring>()] = proxy_cj["headers"][h]["value"].to<nstring>();
                m_proxy.append(proxy);
            }
   //     }
     //   else
       // {
        //    std::cout << "proxy start failed" << std::endl;
       // }
        
    }

    m_info.gzip = config["gzip"].to<bool>();
    m_info.rootdir = config["rootdir"].to<nstring>();
    m_info.default_codec = config["default_codec"].to<nstring>();
    m_info.download_maxbaud = config["download_maxbaud"].to<uint32>();

    return true;
}

void newobj::network::http::website::close()
{
}
network::http::router* network::http::website::router()
{
    return m_router;
}

network::http::session_mgr* newobj::network::http::website::session()
{
    return m_session;
}

bool newobj::network::http::website::host(const nstring& host)
{
    for (size_t i = 0; i < m_hosts.size(); i++)
    {
        //nstring hostname = m_hosts[i]->hostname();
        if (m_hosts[i]->hostname().equals(host) || m_hosts[i]->hostname().find("0.0.0.0") != -1)
            return true;
    }
    return false;
}

newobj::nolock_array<network::http::proxy*>* newobj::network::http::website::proxy()
{
    return &m_proxy;
}

const network::http::website_info* newobj::network::http::website::info()
{
    return &m_info;
}
#endif
