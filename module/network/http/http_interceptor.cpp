#include "http_interceptor.h"
#include "http_reqpack.h"
#include "http_request.h"
#include <iostream>
#include "http_center.h"
#include "util/time.h"
#if USE_NET_HTTP_WEBSITE
network::http::interceptor::interceptor()
{

}
network::http::interceptor::~interceptor()
{

}
size_t network::http::interceptor::add(const nstring& express, std::function<bool(network::http::reqpack* rp)> callback)
{
    network::http::interceptor_info *info = new network::http::interceptor_info;
    info->express = std::regex(express.c_str());
    info->callback = callback;
    info->express_string = express;
    return m_array.append(info);
}
bool network::http::interceptor::trigger(const nstring& url, network::http::reqpack* rp)
{
	t_ret_t(m_array.m_count == 0);
	for(size_t i=0;i<m_array.m_count;i++)
	{
        auto info = m_array.get(i);
        if(std::regex_match(url.c_str(),info->express)){
            bool result = info->callback(rp);
            if(result == false){
#if HTTP_INTERCEPTOR_PRINT == 1
                newobj::log->warn("["+rp->exec_msec()+" ms] false url:"+url+"\t"+" express:"+info->express_string+" ip:"+rp->request()->remote_ipaddress(true),"interceptor");
#endif
            }
            return result;
        }
    }
    return true;
}
#endif
