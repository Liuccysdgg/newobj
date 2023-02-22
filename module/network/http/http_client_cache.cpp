#include "http_client_cache.h"
#if USE_NET_HTTP_CLIENT
#include "http_client.h"
#include "http_client_plus.h"
#include "util/forstring.h"
#include "util/codec.h"
#include "util/file.h"
newobj::network::http::client_cache::client_cache()
{
    m_open = false;
}

newobj::network::http::client_cache::~client_cache()
{
}

void newobj::network::http::client_cache::open(const nstring& dirpath)
{
    m_dirpath = dirpath;
    m_open = true;
    newobj::file::create_dir(m_dirpath,true);
}
#ifdef LIB_CURL
bool newobj::network::http::client_cache::read(newobj::network::http::client* client,newobj::buffer& cache)
{
    if (client->status() != 304)
        return false;
    nstring cache_file_path = m_dirpath + "\\" + codec::md5(client->m_url);
    nstring cache_date_path = m_dirpath + "\\" + codec::md5(client->m_url) + "_lastmodified.txt";
    
    if (newobj::file::exist(cache_date_path) == false || newobj::file::exist(cache_file_path) == false)
        return false;

    newobj::buffer cache_data;
    newobj::file::read(cache_date_path, cache_data);
    if (cache_data.to_string() != client->headers_response()->get("Last-Modified").to_string())
        return false;
    return newobj::file::read(cache_file_path, cache);
}
#endif
bool newobj::network::http::client_cache::read(newobj::network::http::client_plus* client, newobj::buffer& cache)
{
    if (client->status() != 304)
        return false;
    nstring cache_file_path = m_dirpath + "\\" + codec::md5(client->url());
    nstring cache_date_path = m_dirpath + "\\" + codec::md5(client->url()) + "_lastmodified.txt";

    if (newobj::file::exist(cache_date_path) == false || newobj::file::exist(cache_file_path) == false)
        return false;

    newobj::buffer cache_data;
    newobj::file::read(cache_date_path, cache_data);
    if (cache_data.to_string() != client->headers_response().get("Last-Modified").to_string())
        return false;
    return newobj::file::read(cache_file_path, cache);
}
#ifdef LIB_CURL
void newobj::network::http::client_cache::set_header(newobj::network::http::client* client, const nstring& url)
{
    nstring cache_file_path = m_dirpath + "\\" + codec::md5(url);
    nstring cache_date_path = m_dirpath + "\\" + codec::md5(url) + "_lastmodified.txt";

    if (newobj::file::exist(cache_date_path) == false || newobj::file::exist(cache_file_path) == false)
        return ;
    newobj::buffer cache_data;
    newobj::file::read(cache_date_path, cache_data);
    client->header("If-Modified-Since",cache_data.to_string());
}
#endif
void newobj::network::http::client_cache::set_header(newobj::network::http::client_plus* client, const nstring& url)
{
    nstring cache_file_path = m_dirpath + "\\" + codec::md5(url);
    nstring cache_date_path = m_dirpath + "\\" + codec::md5(url) + "_lastmodified.txt";

    if (newobj::file::exist(cache_date_path) == false || newobj::file::exist(cache_file_path) == false)
        return;
    newobj::buffer cache_data;
    newobj::file::read(cache_date_path, cache_data);
    client->headers_request().set("If-Modified-Since", cache_data.to_string());
}

void newobj::network::http::client_cache::close()
{
    m_open = false;
}
#ifdef LIB_CURL
void newobj::network::http::client_cache::write(newobj::network::http::client* client)
{
    auto header = client->headers_response()->get("Cache-Control");
    if (!header.exist("no-cache"))
        return;
    if (client->status() == 304)
        return;
    nstring cache_file_path = m_dirpath + "\\" + codec::md5(client->m_url);
    nstring cache_date_path = m_dirpath + "\\" + codec::md5(client->m_url)+"_lastmodified.txt";

    if (newobj::file::exist(cache_date_path))
    {
        newobj::buffer cache_data;
        newobj::file::read(cache_date_path,cache_data);
        if (cache_data.to_string() == client->headers_response()->get("Last-Modified").to_string())
            return;
    }
    newobj::file::write(cache_date_path, client->headers_response()->get("Last-Modified").to_string());
    newobj::file::write(cache_file_path, client->response());
}
#endif
void newobj::network::http::client_cache::write(newobj::network::http::client_plus* client)
{
    auto header = client->headers_response().get("Cache-Control");
    if (!header.exist("no-cache"))
        return;
    if (client->status() == 304)
        return;
    nstring cache_file_path = m_dirpath + "\\" + codec::md5(client->url());
    nstring cache_date_path = m_dirpath + "\\" + codec::md5(client->url()) + "_lastmodified.txt";

    if (newobj::file::exist(cache_date_path))
    {
        newobj::buffer cache_data;
        newobj::file::read(cache_date_path, cache_data);
        if (cache_data.to_string() == client->headers_response().get("Last-Modified").to_string())
            return;
    }
    newobj::file::write(cache_date_path, client->headers_response().get("Last-Modified").to_string());
    newobj::file::write(cache_file_path, client->response());
}

bool newobj::network::http::client_cache::read(const nstring& url, newobj::buffer& cache)
{
    nstring cache_file_path = m_dirpath + "\\" + codec::md5(url);
    f_ret_f(newobj::file::exist(cache_file_path));
    return newobj::file::read(cache_file_path,cache);
}
#endif