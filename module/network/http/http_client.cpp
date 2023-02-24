#if USE_NET_HTTP_CLIENT
#ifdef LIB_CURL
#include "http_client.h"
#include "public/define.h"
#include "util/forstring.h"
#include "util/file.h"
#include "util/codec.h"
#include "util/time.h"
#include "public/exception.h"
#include "curl/curl.h"
#include <iostream>
#include "http_client_cache.h"
#if LIB_QT == 1
#ifdef QT5
#include "QtCore/qtimer.h"
#include "QtCore/qdatetime.h"
#include "QtCore/qcoreapplication.h"
#else
#include "Qt/qtimer.h"
#include "qt/qdatetime.h"
#include "qt/qcoreapplication.h"
#endif
#endif
int http_client_curl_progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    auto obj = (newobj::network::http::client*)ptr;
    if(obj->m_download_callback_progress != nullptr)
    {
        return obj->m_download_callback_progress(*obj,TotalToDownload, NowDownloaded, TotalToUpload, NowUploaded);
    }
    return 0;
}
size_t http_client_curl_easy_post_write_data(void* buffer, size_t size, size_t nmemb, void* stream)
{
    auto obj = (newobj::network::http::client*)stream;
   
    if (obj->m_download_callback != nullptr)
    {
        obj->m_download_callback(buffer,size*nmemb,*obj);
    }
    else
    {
        obj->m_response.append((uchar*)buffer, size * nmemb);
    }
    return size * nmemb;
}
size_t http_client_curl_easy_read_headers(void* buffer, size_t size, size_t nmemb, void* stream)
{
#define RETURN_OVER return size * nmemb
    auto obj = (newobj::network::http::client*)stream;
    nstring head = std::string((char*)buffer, size * nmemb);
    try
    {
        /*最头部*/
        {
            // "HTTP/1.1 200 OK\r\n"
            if (head.length()>=5 && head.left(5) == "HTTP/")
            {
                auto list = head.split(" ");
                if (list.size() >= 3)
                {
                    obj->m_status = list[1].to_int32();
                    RETURN_OVER;
                }
            }
        }
        /*协议头*/
        int32 idx = head.find(": ");
        if (idx != -1)
        {
            nstring name = head.left(idx);
            nstring value = head.right(head.length() - idx - 2).trim_end('\n').trim_end('\r');

            
  /*          if (obj->m_headers_response.exist(name))
            {
                auto header = obj->m_headers_response.get(name);
                header.push(value);
                obj->m_headers_response.set(name, header);
            }
            else
            {*/
                auto header = obj->m_headers_response.get(name);
                header.parse(value);
                obj->m_headers_response.set(name, header);
     //       }
        }

    }
    catch (const newobj::exception& e)
    {
        e.print();
    }
    RETURN_OVER;
}

void newobj::network::http::client::init()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

newobj::network::http::client::client()
{
    m_download_callback = nullptr;
    m_download_callback_end = nullptr;
    m_download_callback_failed = nullptr;
    m_status = 0;
    m_timeout = 3000;
    m_qt_prog = false;
    m_cache = nullptr;
    m_proxy_port = 0;
}
int32 newobj::network::http::client::status()
{
    return m_status;
}
newobj::network::http::client::~client()
{
}
void newobj::network::http::client::set_timeout(uint32 timeout_msec)
{
    m_timeout = timeout_msec;
}

bool newobj::network::http::client::post(const nstring& url, const newobj::buffer& reqdata, bool exec_now, bool is_put)
{
    if(is_put)
        m_method = PUT;
    else
        m_method = POST;
    m_url = url;
    
    m_request = reqdata;
    return request(exec_now);
}

bool newobj::network::http::client::post(const nstring& url, const newobj::json& reqjson, bool exec_now, bool to_utf8, bool is_put)
{
    header("Content-Type", "application/json");
    if(to_utf8)
        return post(url, newobj::buffer(codec::to_utf8(reqjson.to_string())), exec_now,is_put);
    else
        return post(url,newobj::buffer(reqjson.to_string()),exec_now, is_put);
}

bool newobj::network::http::client::post(const nstring& url, const std::map<nstring, nstring>& request, bool to_utf8, bool exec_now, bool is_put)
{
    
    nstring param_str;
    for_iter(iter, request)
        param_str += (param_str.length() == 0 ? "" : "&") + iter->first + "=" + iter->second;
    if (to_utf8)
    {
        param_str = codec::to_utf8(param_str);
        header("Content-Type", "application/x-www-form-urlencoded;charset=utf-8");
    }
    else
    {
        header("Content-Type", "application/x-www-form-urlencoded");
    }
        
    return post(url,newobj::buffer(param_str),exec_now, is_put);
}

bool newobj::network::http::client::post(const nstring& url, const http::make_form& request, bool exec_now, bool is_put)
{
    newobj::buffer data;
    nstring boundary;
    request.make(data,boundary);
    header("Content-Type","boundary="+boundary+"; multipart/form-data");
    return post(url, data, exec_now);
}

bool newobj::network::http::client::get(const nstring& url, bool exec_now)
{
    m_method = GET;
    m_url = url;
    return request(exec_now);
}

bool newobj::network::http::client::get(const nstring& url, const std::map<nstring, nstring>& request, bool exec_now)
{
    nstring request_url;
    nstring param_str;
    for_iter(iter, request)
        param_str += (param_str.length() == 0 ? "" : "&") + iter->first + "=" + iter->second;
    request_url = nstring(url + (param_str.length() == 0 ? "" : "?") + param_str);
    return get(request_url, exec_now);
}

const newobj::buffer& newobj::network::http::client::response()
{
    return m_response;
}

void newobj::network::http::client::header(const nstring& title, const nstring& content)
{
    /*Cookie*/
    m_headers_request.set(title, content);
}

newobj::network::http::header newobj::network::http::client::header(const nstring& title)
{
    return m_headers_response.get(title);
}

network::http::header_list* newobj::network::http::client::headers_request()
{
    return &m_headers_request;
}

network::http::header_list* newobj::network::http::client::headers_response()
{
    return &m_headers_response;
}

network::http::cookie& newobj::network::http::client::cookie()
{
    return m_cookie;
}

void newobj::network::http::client::cookie(const network::http::cookie& ck)
{
    this->m_cookie = ck;
}

void newobj::network::http::client::proxy(const nstring& address, uint32 port)
{
    m_proxy = address;
    m_proxy_port = port;
}


void newobj::network::http::client::on_down_end(const std::function<void(newobj::network::http::client& client)>& callback)
{
    m_download_callback_end = callback;
}

void newobj::network::http::client::on_down_failed(const std::function<void(newobj::network::http::client& client)>& callback)
{
    m_download_callback_failed = callback;
}
void newobj::network::http::client::on_down_progress(const std::function<int(newobj::network::http::client& client, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)>& callback)
{
    this->m_download_callback_progress = callback;
}
bool newobj::network::http::client::qt()
{
    return m_qt_prog;
}
void newobj::network::http::client::cache(client_cache* cache)
{
    m_cache = cache;
}
void newobj::network::http::client::qt(bool enable)
{
    this->m_qt_prog = enable;
}
void newobj::network::http::client::on_down_ing(const std::function<void(void* data, uint32 downsize, newobj::network::http::client& client)>& callback)
{
    this->m_download_callback = callback;
}

bool newobj::network::http::client::curl()
{
    bool result = false;
    CURL* curl = nullptr;
    CURLcode res;
    struct curl_slist* headers = NULL; /* init to NULL is important */

    curl = curl_easy_init();
    if (curl == nullptr)
    {
        m_lastErrorDesc = "curl 初始化失败";
        return false;
    }
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_timeout);
    curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());

    if (m_method == POST || m_method == PUT)
    {
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, m_request.length());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_request.data());
        if (m_method == POST)
        {
            
            curl_easy_setopt(curl, CURLOPT_POST, 1);
        }
        else if (m_method == PUT)
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            //curl_easy_setopt(curl, CURLOPT_PUT, 1);
        }
        
    }
    else if (m_method == GET)
    {
        //curl_easy_setopt(curl, CURLOPT_POST, 0);
    }
    
    if (m_proxy != "")
    {
        curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_BASIC); //代理认证模式
        curl_easy_setopt(curl, CURLOPT_PROXY, m_proxy.c_str()); //代理服务器地址
        curl_easy_setopt(curl, CURLOPT_PROXYPORT, m_proxy_port); //代理服务器端口
        curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP); //使用http代理模式
    }


    /*header*/
    {
        auto map = m_headers_request.to();
        for_iter(iter, map)
            headers = curl_slist_append(headers, nstring(iter->first+ ": " + iter->second).c_str());
        curl_slist_append(headers, "Expect: ");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_client_curl_easy_post_write_data); //对返回的数据进行操作的函数地址
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http_client_curl_easy_read_headers); //对返回的头部进行操作的函数地址

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, http_client_curl_progress_func);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); //这是write_data的第四个参数值
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, this); //这是write_data的第四个参数值
        //跳过对ca的检查，简单
#define  SKIP_PEER_VERIFICATION
#ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
#endif
        //跳过检查
#define SKIP_HOSTNAME_VERIFICATION
#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
#endif
        //curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS, CURLSSLOPT_ALLOW_BEAST);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
    {
        result = false;
        auto c_err = curl_easy_strerror(res);
        if (c_err != nullptr)
            m_lastErrorDesc = c_err;
    }
    else
    {

        result = true;
    }
    /* always cleanup */
    curl_slist_free_all(headers); /* free the header list */
    curl_easy_cleanup(curl);

    return result;
}

bool newobj::network::http::client::request(bool exec_now)
{
    if (!exec_now)
        return true;
    /*初始化环境*/
    m_response.clear();
    m_status = 0;
    m_headers_response.clear();

    if (m_cookie.to_string() != "")
        m_headers_request.set("Cookie", m_cookie.to_string());

    /*【缓存】置头*/
    if (m_cache != nullptr && m_method == GET)
    {
        m_cache->set_header(this,m_url);
    }

    // CURL网络请求
#if defined(LIB_QT)
    
    bool result = false;
    if (!m_qt_prog)
    {
        result = curl();
    }
    else
    {
        bool thread_over = false;
        newobj::thread::create([&](void* param) {
            result = curl();
            thread_over = true;
        }, nullptr, false);
        while (!thread_over)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }
#else
    bool result = curl();
#endif

    if (!result)
    {
        if (m_download_callback_failed != nullptr)
            m_download_callback_failed(*this);
    }
    else
    {
        if (m_download_callback_end != nullptr)
            m_download_callback_end(*this);
    }


    /*【缓存】判断缓存*/
    if (m_cache != nullptr && m_method == GET)
    {
        bool cache = false;
        if (this->status() == 304)
        {
            cache = m_cache->read(this,m_response);
            if (cache)
            {
                std::cout << "[CURL] [CACHE] "<< m_url.c_str() << std::endl;
            }
            else
            {
                std::cout << "[CURL] [CACHE_FAILED] " << m_url.c_str() << std::endl;
            }
        }
        if (cache == false)
        {
            std::cout << "[CURL] [REMOTE] " << m_url.c_str() << std::endl;
            m_cache->write(this);
        }
    }
    /*合并响应cookie*/
    {
        if (m_headers_response.exist("Set-Cookie") != false && m_headers_response.get("Set-Cookie").to_string() != "")
            m_cookie.merge(m_headers_response.get("Set-Cookie").to_string());
    }
    return result;
}

#endif
#endif