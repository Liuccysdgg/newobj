#include "http_cookie.h"
#if USE_NET_HTTP_UTIL
#include "util/forstring.h"
newobj::network::http::cookie::cookie()
{

}

newobj::network::http::cookie::~cookie()
{

}

void newobj::network::http::cookie::merge(const cookie& ck)
{
    for_iter(iter, ck.m_param)
        m_param[iter->first] = iter->second;
}
void newobj::network::http::cookie::merge(const nstring& set_cookie)
{
    if (set_cookie == "")
        return;
    std::map<nstring, nstring> set_cookie_map;
    std::map<nstring, nstring> local_cookie_map;
    std::vector<nstring_view> cookies = set_cookie.split(";");
    if (cookies.size() == 0)
    {
        auto arr = set_cookie.split("=");
        if(arr.size() == 2)
            m_param[arr[0]] = arr[1];
    }
    else
    {
        for (uint32 i = 0; i < cookies.size(); i++)
        {
            auto arr = cookies[i].split("=");
            if (arr.size() == 2)
                m_param[arr[0].trim(' ')] = arr[1].trim(' ');
        }
    }
    

}

nstring newobj::network::http::cookie::to_string()
{
    nstring cookie_string;
    uint32 idx = 0;
    for_iter(iter, m_param)
    {
        if (iter->first == "")
            continue;
        if (idx == m_param.size() - 1)
        {
            cookie_string += iter->first + "=" + iter->second;
        }
        else
        {
            cookie_string += iter->first + "=" + iter->second + "; ";
        }
    }
    return cookie_string.trim_end(' ').trim_end(';');
}
void newobj::network::http::cookie::clear()
{
    m_param.clear();
}
#endif