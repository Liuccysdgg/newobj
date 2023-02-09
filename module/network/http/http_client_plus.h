#pragma once
#include "http_define.h"
#if USE_NET_HTTP_CLIENT
#include "public/string.h"
#include "public/base.h"
#include "util/json.h"

#include "http_header.h"
#include "http_client_cache.h"
#include "http_cookie.h"
#include "make_form.h"
class http_client_listener;
namespace newobj
{
	namespace network
	{
		namespace http
		{
			class MNETWORK_API client_plus:public newobj::base
			{
			public:
				client_plus();
				~client_plus();
				void close();
				void set_timeout(uint32 connect_msec = 3000,uint32 recv_msec = 8000);
				bool get(const nstring& url,const std::map<nstring,nstring>& value = std::map<nstring,nstring>()); 
				bool post(const nstring& url,const std::map<nstring,nstring>& value,bool to_utf8 = false);
				bool post(const nstring& url, const newobj::json& value, bool to_utf8 = false);
				bool post(const nstring& url, const newobj::buffer& value);
				bool post(const nstring& url, const http::make_form& value);
				network::http::header_list& headers_request();
				network::http::header_list& headers_response();
				uint32 status();
				newobj::buffer& response();
				/*设置缓存*/
				void cache(client_cache* cache);
				/*请求路径*/
				inline const nstring& url() { return m_url; }
				/*cookie*/
				inline network::http::cookie& cookie() { return m_cookie; }
				inline void cookie(const network::http::cookie& ck) { m_cookie = ck; }

				/*[回调] 正在下载*/
				void on_down_ing(const std::function<bool(void* data, uint32 downsize, uint64 allsize, newobj::network::http::client_plus& client)>& callback);
				/*[回调] 下载结束*/
				void on_down_end(const std::function<void(newobj::network::http::client_plus& client)>& callback);
				/*[回调] 下载失败*/
				void on_down_failed(const std::function<void(newobj::network::http::client_plus& client)>& callback);

				friend class http_client_listener;
			private:
				bool parseurl(const nstring& url);
				bool connect();
				bool init();
				bool request();
				bool post(const nstring& url);
				void* client();
			private:
				// HP客户端
				void* m_client;
				// HP客户端-SSL
				void* m_client_ssl;
				// HP监听器
				http_client_listener* m_listener;
				// 是否已初始化
				bool m_init;
				// 连接IP
				nstring m_ipaddress;
				// 连接端口
				ushort m_port;
				// HTTPS
				bool m_ssl;
				// 请求链接
				nstring m_url;
				// 请求路径
				nstring m_path;
				// 请求方式
				network::http::method m_method;
				// 请求数据
				newobj::buffer m_request_body;
				// [header] 请求
				network::http::header_list m_headers_request;
				// 超时时间 连接
				timestamp m_timeout_connect_msec;
				// 超时时间 接收
				timestamp m_timeout_recv_msec;
				// 缓存
				client_cache* m_cache;
				// cookie
				network::http::cookie m_cookie;
#ifndef _WIN32
			public:
#endif
				// 关闭
				bool m_close;

			};
		}
	}
}
#endif