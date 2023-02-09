#include "http_parser.h"
#if USE_NET_HTTP_WEBSITE
#include "util/file.h"

network::http::parser::parser()
{
    m_data = nullptr;
    m_method = network::http::ALL;
}

network::http::parser::~parser()
{

}

const newobj::json& newobj::network::http::parser::json()
{
    return m_json_param;
}

nstring newobj::network::http::parser::text()
{
    return m_data->to_string();
}

bool network::http::parser::init(const nstring &url,const network::http::method& method, buffer *data, const nstring &content_type)
{
    m_url = url;
    m_data = data;
    m_content_type = content_type;
    m_method = method;


    /*解析URL*/
    parse_url(m_url,m_url_param);
    switch (m_method) {
    case network::http::GET:
        break;
    case network::http::POST:
        {
            if(content_type == "application/x-www-form-urlencoded")
            {
                parse_url(m_data->to_string(),m_body_param);
            }
            else if(content_type.find("application/json") != -1)
            {
                parse_json();
            }
            else if(content_type.find("multipart/form-data") !=-1)
            {
                parse_form();
            }
        }
        break;
    default:
        break;
    }
    return true;
}

bool network::http::parser::url_param(const nstring &name, nstring &value)
{
        return read_url_param(m_url_param,name,value);
}

bool newobj::network::http::parser::url_param_exist(const nstring& name)
{
    return m_url_param.find(name) != m_url_param.end();
}

std::vector<nstring> network::http::parser::url_param_names()
{
    std::vector<nstring> names;
    for_iter(iter,m_url_param)
        names.push_back(iter->first);
    return names;
}

bool network::http::parser::body_param(const nstring &name, nstring &value)
{
    if(m_method == POST)
    {
        if(m_content_type == "application/x-www-form-urlencoded")
        {
            return read_url_param(m_body_param,name,value);
        }
        else if(m_content_type == "application/json")
        {
            f_ret_f(m_json_param.exist(name));
            if(m_json_param[name].is_number()){
                value = nstring::from(m_json_param[name].to<double>());
            }else{
                value = m_json_param[name].to<nstring>();
            }
            return true;
        }
        return false;
    }
    return false;
}

bool newobj::network::http::parser::body_param_exist(const nstring& name)
{
    if (m_method == POST)
    {
        if (m_content_type == "application/x-www-form-urlencoded")
        {
            return m_body_param.find(name) != m_body_param.end();
        }
        else if (m_content_type == "application/json")
        {
            return m_json_param.exist(name);
        }
    }
    return false;
}

std::vector<nstring> network::http::parser::body_param_names()
{

    std::vector<nstring> names;
    if(m_method == POST)
    {
        if(m_content_type == "application/x-www-form-urlencoded")
        {
            for_iter(iter,m_body_param)
                names.push_back(iter->first);
        }
        else if(m_content_type == "application/json")
        {
            return m_json_param.keys();
        }
    }
    return names;
}

network::http::form_parser *network::http::parser::form()
{
    return &m_form;
}

void network::http::parser::parse_url(const nstring &url, std::map<nstring,nstring> &map)
{
    map.clear();
    std::vector<nstring> ps = url.split('&');
    for(size_t i=0;i<ps.size();i++)
    {
        std::vector<nstring> pi = ps[i].split('=');
        if(pi.size() == 2)
            map.insert(std::pair<nstring,nstring>(pi[0],pi[1]));
    }
}
void network::http::parser::parse_json()
{
    m_json_param.parse(m_data->to_string());
}

void network::http::parser::parse_form()
{
    m_form.parse(m_data);
}

bool network::http::parser::read_url_param(const std::map<nstring,nstring> &map, const nstring &name, nstring &value)
{
    auto iter = map.find(name);
    t_ret_f(iter == map.end());
    value = iter->second;
    return true;
}

network::http::form_parser::form_parser()
{

}

network::http::form_parser::~form_parser()
{

}

std::vector<nstring> network::http::form_parser::names()
{
    std::vector<nstring> result;
    for_iter(iter,m_infos)
        result.push_back(iter->first);
    return result;
}

bool network::http::form_parser::get(const nstring &name, form_info &info)
{
    auto iter = m_infos.find(name);
    t_ret_f(iter == m_infos.end());
    info = iter->second;
    return true;
}

bool network::http::form_parser::write_file(const nstring &name, const nstring &filepath)
{
    auto iter = m_infos.find(name);
    t_ret_f(iter == m_infos.end());
    return newobj::file::write(filepath,(const char*)m_data->data()+iter->second.start,iter->second.length);
}

bool network::http::form_parser::parse(buffer *data)
{
    this->m_data = data;

    std::vector<uint32> starts;
    std::vector<uint32> lengths;
    parse_count(starts,lengths);


    for(size_t i=0;i<starts.size();i++)
        parse_form(starts[i],lengths[i]);

    return true;
}

void network::http::form_parser::parse_count(std::vector<uint32> &starts, std::vector<uint32> &lengths)
{


    newobj::buffer cut_flag;
    {
        int32 curr_idx = 0;
        while (char c = getchar(curr_idx++))
        {
            if (c == '\r' && getchar(curr_idx) == '\n')
            {
                curr_idx++;
                break;
            }
            else
                cut_flag.append(c);
        }
    }

    auto idxVct = m_data->cut(cut_flag);
    for (size_t i = 0; i < idxVct.size(); i++)
    {
        int32 startIdx = idxVct[i] + cut_flag.length() + 2;
        if (i + 1 != idxVct.size())
        {
            starts.push_back(startIdx);
            lengths.push_back(idxVct[i + 1] - idxVct[i] - cut_flag.length() - 2);
        }
    }
}

void network::http::form_parser::parse_form(uint32 start,uint32 length)
{
    auto parse_paraminfo_secondkv=[](buffer buf)->Kv
    {
        Kv kv;
        buffer cutV;
        cutV.setsize(1);
        cutV[0] = '=';
        auto cdIdx = buf.find(0, cutV);
        if (cdIdx == -1)
            return kv;
        kv.name = buf.sub(0, cdIdx).to_string();
        kv.value = buf.sub(cdIdx + 2, buf.length() - 3 - cdIdx).to_string();
        return kv;
    };


    newobj::buffer form_data;

    form_data.__set((uchar*)m_data->data()+start,length);
    form_data.deepcopy(false);


    newobj::buffer fh;
    newobj::buffer fh2;
    newobj::buffer cs_ln;
    fh.setsize(2);
    fh2.setsize(2);
    cs_ln.setsize(2);
    fh[0] = ';';
    fh[1] = ' ';
    fh2[0] = ':';
    fh2[1] = ' ';
    cs_ln[0] = '\r';
    cs_ln[1] = '\n';

    auto div = form_data.cut(cs_ln);
    if (div.size() == -1)
        return;

    form_info forminfo;
    //数据
    if (div.size() == 3)
    {
        forminfo.start = div[1] + 2+start;
        forminfo.length = div[div.size() - 1] - 2 - div[1];
    }
    else if (div.size() > 3)
    {
        forminfo.start = div[2] + 2+start;
        forminfo.length = div[div.size() - 1] - 2 - div[2];

        auto ContentType = form_data.sub(div[0] + 2, div[1] - 2 - div[0]);
        // Content-Type
        auto cdIdx = ContentType.find(0, fh2);
        if (cdIdx == -1)
            return;
        forminfo.content_type = ContentType.sub(cdIdx + 2, ContentType.length() - 2 - cdIdx).to_string();
    }
    else
        return;

    //配置项
    auto info = form_data.sub(0, div[0]);
    nstring t = info.to_string();
    auto info_vct = info.cut(fh);
    if (info_vct.size() == 0)
        return;

    // Content-Disposition
    auto contentDisposition = info.sub(0, info_vct[0]);
    auto cdIdx = contentDisposition.find(0, fh2);
    if (cdIdx == -1)
        return;

    forminfo.disposition = contentDisposition.sub(cdIdx + 2, contentDisposition.length() - 2 - cdIdx).to_string();



    if (info_vct.size() == 1)
    {
        auto kv = parse_paraminfo_secondkv(info.sub(info_vct[0] + 2, info.length() - info_vct[0] - 2));
        if (kv.name == "name")
            forminfo.name = kv.value;
        else if (kv.name == "filename")
            forminfo.filename = kv.value;
    }
    else
    {
        for (size_t i = 0; i < info_vct.size(); i++)
        {
            newobj::buffer info_sub;
            if (info_vct.size() == i + 1)
                info_sub = info.sub(info_vct[i] + 2, info.length() - info_vct[i] - 2);
            else
                info_sub = info.sub(info_vct[i] + 2, info_vct[i + 1] - info_vct[i] - 2);
            auto kv = parse_paraminfo_secondkv(info_sub);
            if (kv.name == "name")
                forminfo.name = kv.value;
            else if (kv.name == "filename")
                forminfo.filename = kv.value;
        }
    }
    m_infos.insert(std::pair<nstring,form_info>(forminfo.name,forminfo));
}

char network::http::form_parser::getchar(size_t index)
{
    if (index + 1 > m_data->length())
        throw 0;
    return *(m_data->data() + index);
}
#endif