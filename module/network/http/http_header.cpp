#include "http_header.h"
#if USE_NET_HTTP_UTIL

newobj::network::http::header::header()
{
}

newobj::network::http::header::~header()
{
}

void newobj::network::http::header::clear()
{
    m_param.clear();
    m_values.clear();
    m_name = "";
}

void newobj::network::http::header::name(const nstring& value)
{
	m_name = value;
}
nstring newobj::network::http::header::name() const
{
    return m_name;
}


std::map<nstring, nstring> newobj::network::http::header::param()
{
    // TODO: 在此处插入 return 语句
    return m_param;
}

void network::http::header::parse(const nstring &body)
{
    m_values.clear();
    m_param.clear();
    nstring value = body;
    std::vector<stream_view> param = value.split_view(nstring_view(";"));
    if (param.size() == 0)
    {
        value = value.trim(' ');
        auto arr = value.split("=");
        if (arr.size() == 2)
            m_param[arr[0]] = arr[1];
    }
    else
    {
        for (uint32 i = 0; i < param.size(); i++)
        {
            param[i] = param[i].trim(' ');
            auto arr = param[i].split_view(nstring_view("="));
            if (arr.size() != 2)
                m_values.push_back(param[i]);
            else
                m_param[arr[0]] = arr[1];
        }
    }
}
void newobj::network::http::header::push(const nstring& value)
{
    m_values.push_back(value);
}

void network::http::header::push(const nstring &key, const nstring &value)
{
    m_param[key] = value;
}
bool newobj::network::http::header::exist(const nstring& name)
{
    
    if ((m_param.find(name) != m_param.end()) == false)
    {
        for (size_t i = 0; i < m_values.size(); i++)
        {
            if (m_values[i] == name)
                return true;
        }
        return false;
    }
    return true;
}

nstring newobj::network::http::header::to_string() const
{
    nstring value;
    for_iter(iter, m_values)
        value.append(*iter + "; ");

    for_iter(iter, m_param)
    {
        nstring line = nstring(iter->first + "=" + iter->second + ";");
        value.append(line);
    }
        

    
    return value.trim_end(' ').trim_end(';');
}

newobj::network::http::header_list::header_list()
{

}

newobj::network::http::header_list::~header_list()
{
    clear();
}

bool newobj::network::http::header_list::exist(const nstring& name) const
{
    return m_headers.find(name) != m_headers.end();
}

newobj::network::http::header newobj::network::http::header_list::get(const nstring& name)
{
    auto iter = m_headers.find(name);
    if (iter == m_headers.end())
    {
        m_headers[name].name(name);
    }
    return m_headers[name];
}

void newobj::network::http::header_list::set(const nstring& name, const newobj::network::http::header& header)
{
    m_headers[name] = header;
}

void newobj::network::http::header_list::set(const nstring& name, const nstring& value)
{
    m_headers[name].parse(value);
}
void newobj::network::http::header_list::del(const nstring& name)
{
    auto iter = m_headers.find(name);
    if (iter == m_headers.end())
        return;
    m_headers.erase(iter);
}

uint32 newobj::network::http::header_list::size()
{
    return m_headers.size();
}

void newobj::network::http::header_list::clear()
{
    m_headers.clear();
}

std::map<nstring, nstring> newobj::network::http::header_list::to() const
{
    std::map <nstring, nstring> result;
    for_iter(iter, m_headers)
    {
        result[iter->first] = iter->second.to_string();
        
    }
        
    return result;
}
#endif