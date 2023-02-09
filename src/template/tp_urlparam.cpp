#include "tp_urlparam.h"
#ifndef MSVC_2010
web::urlparam::urlparam(const std::initializer_list<Kv> &kvs)
{
    this->m_kvs = kvs;
}

web::urlparam::urlparam()
{
}

web::urlparam::~urlparam()
{
}

void web::urlparam::operator=(const std::initializer_list<Kv>& kvs)
{
    
        this->m_kvs = kvs;
    
}

nstring web::urlparam::to_string()
{
    nstring str;
    for (uint32 i = 0; i < m_kvs.size(); i++)
    {
        if ((i + 1) == m_kvs.size())
        {
            str += m_kvs[i].name + "=" + m_kvs[i].value;
        }
        else
        {
            str += m_kvs[i].name + "=" + m_kvs[i].value + "&";
        }
    }
    return str;
}

void web::urlparam::append(const Kv& kv)
{
    this->m_kvs.push_back(kv);
}
#endif