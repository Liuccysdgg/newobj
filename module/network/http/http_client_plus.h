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
				/*���û���*/
				void cache(client_cache* cache);
				/*����·��*/
				inline const nstring& url() { return m_url; }
				/*cookie*/
				inline network::http::cookie& cookie() { return m_cookie; }
				inline void cookie(const network::http::cookie& ck) { m_cookie = ck; }

				/*[�ص�] ��������*/
				void on_down_ing(const std::function<bool(void* data, uint32 downsize, uint64 allsize, newobj::network::http::client_plus& client)>& callback);
				/*[�ص�] ���ؽ���*/
				void on_down_end(const std::function<void(newobj::network::http::client_plus& client)>& callback);
				/*[�ص�] ����ʧ��*/
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
				// HP�ͻ���
				void* m_client;
				// HP�ͻ���-SSL
				void* m_client_ssl;
				// HP������
				http_client_listener* m_listener;
				// �Ƿ��ѳ�ʼ��
				bool m_init;
				// ����IP
				nstring m_ipaddress;
				// ���Ӷ˿�
				ushort m_port;
				// HTTPS
				bool m_ssl;
				// ��������
				nstring m_url;
				// ����·��
				nstring m_path;
				// ����ʽ
				network::http::method m_method;
				// ��������
				newobj::buffer m_request_body;
				// [header] ����
				network::http::header_list m_headers_request;
				// ��ʱʱ�� ����
				timestamp m_timeout_connect_msec;
				// ��ʱʱ�� ����
				timestamp m_timeout_recv_msec;
				// ����
				client_cache* m_cache;
				// cookie
				network::http::cookie m_cookie;
#ifndef _WIN32
			public:
#endif
				// �ر�
				bool m_close;

			};
		}
	}
}
#endif