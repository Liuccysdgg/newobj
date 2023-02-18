#include "http_reqpack.h"
#if USE_NET_HTTP_WEBSITE
#include "http_server.h"

#include "http_request.h"
#include "http_response.h"
#include "util/time.h"
#define HPSERVER 
static newobj::object_pool<newobj::network::http::reqpack> m_queue_point("reqpack", POINT_QUEUE_REQPACK_CLEAR_MAX, POINT_QUEUE_REQPACK_CLEAR_SEC);
newobj::network::http::reqpack* newobj::network::http::reqpack::create()
{
    return m_queue_point.create();
}
void newobj::network::http::reqpack::destory(reqpack* p)
{
    //delete p;
    //return;
    if (p->m_data != nullptr)
    {
        delete p->m_data;
        p->m_data = nullptr;
    }
    if (p->m_request != nullptr)
        delete p->m_request;
    if (p->m_response != nullptr)
        delete p->m_response;

    //network::http::request::destory(p->m_request);
    //network::http::response::destory(p->m_response);


    p->m_url.clear();
    p->m_method = network::http::ALL;
    p->m_filepath.clear();
    p->m_referrer.clear();
    p->m_request = nullptr;
    p->m_response = nullptr;
    p->m_data = nullptr;
    p->m_server = nullptr;
    p->m_connid = 0;
    p->m_website = nullptr;
    p->m_begin_msec = 0;

    m_queue_point.destory(p);
}
newobj::network::http::reqpack::reqpack()
{
   
}

newobj::network::http::reqpack::~reqpack()
{

}

void newobj::network::http::reqpack::init(const nstring& url,const nstring& host, newobj::buffer* data, uint64 connid, network::http::server* server)
{
    m_host = host;
    m_url = url;
    m_method = network::http::ALL;
    m_request = nullptr;
    m_response = nullptr;
    m_data = data;
    m_server = server;
    m_connid = connid;
    m_website = nullptr;
    m_begin_msec = time::now_msec();
}

network::http::request *network::http::reqpack::request()
{
    if(m_request == nullptr){
        m_request = new network::http::request;
        m_request->m_reqpack = this;
        m_request->center(m_server->center());
        m_request->website(website());
    }
    return m_request;
}

network::http::response *network::http::reqpack::response()
{
    if(m_response == nullptr){
        m_response = new network::http::response;
        m_response->center(m_server->center());
        m_response->website(website());
        m_response->init(this);
    }
    return m_response;
}
void network::http::reqpack::filepath(const nstring &path)
{
    m_filepath = path;
}
#endif
