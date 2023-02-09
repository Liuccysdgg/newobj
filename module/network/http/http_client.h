#pragma once
#include "http_define.h"
#if USE_NET_HTTP_CLIENT
#include "public/base.h"
#include "public/buffer.h"
#include "util/map.hpp"
#include "public/string.h"
#include "http_cookie.h"
#include "util/json.h"
#include "make_form.h"
#include "util/forstring.h"
#include <functional>
#include "http_header.h"
#ifdef LIB_CURL
typedef void(*CALLBACK_DOWNLOAD_STATUS)(int32 status);
namespace newobj
{
	namespace network
	{
		namespace http
		{
			class client_cache;
			class MNETWORK_API client:public newobj::base
			{
			public:
				static void init();
				client();
				~client();
				/*设置超时*/
				void set_timeout(uint32 timeout_msec);
				/*POST*/
				bool post(const nstring& url,const newobj::buffer& reqdata,bool exec_now = true,bool is_put = false);
				bool post(const nstring& url, const newobj::json& reqjson, bool exec_now = true,bool to_utf8 = false, bool is_put = false);
				bool post(const nstring& url, const std::map<nstring, nstring>& reqparam,bool to_utf8 = false, bool exec_now = true, bool is_put = false);
				bool post(const nstring& url, const http::make_form& reqform, bool exec_now = true, bool is_put = false);
				/*GET*/
				bool get(const nstring& url, bool exec_now = true);
				bool get(const nstring& url,const std::map<nstring,nstring>& request, bool exec_now = true);
				/*取响应数据*/
				const newobj::buffer& response();
				/*置协议头*/
				void header(const nstring& title, const nstring& content);
				/*取协议头*/
				newobj::network::http::header header(const nstring& title);
				/*取状态码*/
				int32 status();
				network::http::header_list* headers_request();
				network::http::header_list* headers_response();
				/*cookie*/
				network::http::cookie& cookie();
				void  cookie(const network::http::cookie& ck);
				/*代理*/
				void proxy(const nstring& address, uint32 port);
				/*[回调] 正在下载*/
				void on_down_ing(const std::function<void(void* data, uint32 downsize, newobj::network::http::client& client)>& callback);
				/*[回调] 下载结束*/
				void on_down_end(const std::function<void(newobj::network::http::client& client)>& callback);
				/*[回调] 下载失败*/
				void on_down_failed(const std::function<void(newobj::network::http::client& client)>& callback);
				/*
					[回调] 下载进度
					@callback
						return：0=成功 -1=停止
				*/
				void on_down_progress(const std::function<int(newobj::network::http::client& client, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)>& callback);
				/*是否为QT程序*/
				void qt(bool enable);
				bool qt();

				/*设置缓存*/
				void cache(client_cache* cache);
				// request
				bool request(bool exec_now);
			private:
				// curl request
				bool curl();
			public:
				// [header] 请求
				network::http::header_list m_headers_request;
				// [header] 响应
				network::http::header_list m_headers_response;
				// cookie
				network::http::cookie m_cookie;
				// 响应
				newobj::buffer m_response;
				// 请求
				newobj::buffer m_request;
				// 超时
				uint32 m_timeout;
				// 状态码
				int32 m_status;
				// 代理IP
				nstring m_proxy;
				// 代理端口
				uint32 m_proxy_port;
				// 请求类型
                http::method m_method;
				// 请求地址
				nstring m_url;
				// 缓存
				client_cache* m_cache;
				// [回调] 下载中
				std::function<void(void * data,uint32 downsize, newobj::network::http::client& client)> m_download_callback;
				std::function<void(newobj::network::http::client& client)> m_download_callback_end;
				std::function<void(newobj::network::http::client& client)> m_download_callback_failed;
				std::function<int(newobj::network::http::client& client, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) > m_download_callback_progress;
			private:
				// QT程序
				bool m_qt_prog;
			};
		}

	}
}
#endif
#endif