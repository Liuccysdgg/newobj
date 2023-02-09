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
				/*���ó�ʱ*/
				void set_timeout(uint32 timeout_msec);
				/*POST*/
				bool post(const nstring& url,const newobj::buffer& reqdata,bool exec_now = true,bool is_put = false);
				bool post(const nstring& url, const newobj::json& reqjson, bool exec_now = true,bool to_utf8 = false, bool is_put = false);
				bool post(const nstring& url, const std::map<nstring, nstring>& reqparam,bool to_utf8 = false, bool exec_now = true, bool is_put = false);
				bool post(const nstring& url, const http::make_form& reqform, bool exec_now = true, bool is_put = false);
				/*GET*/
				bool get(const nstring& url, bool exec_now = true);
				bool get(const nstring& url,const std::map<nstring,nstring>& request, bool exec_now = true);
				/*ȡ��Ӧ����*/
				const newobj::buffer& response();
				/*��Э��ͷ*/
				void header(const nstring& title, const nstring& content);
				/*ȡЭ��ͷ*/
				newobj::network::http::header header(const nstring& title);
				/*ȡ״̬��*/
				int32 status();
				network::http::header_list* headers_request();
				network::http::header_list* headers_response();
				/*cookie*/
				network::http::cookie& cookie();
				void  cookie(const network::http::cookie& ck);
				/*����*/
				void proxy(const nstring& address, uint32 port);
				/*[�ص�] ��������*/
				void on_down_ing(const std::function<void(void* data, uint32 downsize, newobj::network::http::client& client)>& callback);
				/*[�ص�] ���ؽ���*/
				void on_down_end(const std::function<void(newobj::network::http::client& client)>& callback);
				/*[�ص�] ����ʧ��*/
				void on_down_failed(const std::function<void(newobj::network::http::client& client)>& callback);
				/*
					[�ص�] ���ؽ���
					@callback
						return��0=�ɹ� -1=ֹͣ
				*/
				void on_down_progress(const std::function<int(newobj::network::http::client& client, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)>& callback);
				/*�Ƿ�ΪQT����*/
				void qt(bool enable);
				bool qt();

				/*���û���*/
				void cache(client_cache* cache);
				// request
				bool request(bool exec_now);
			private:
				// curl request
				bool curl();
			public:
				// [header] ����
				network::http::header_list m_headers_request;
				// [header] ��Ӧ
				network::http::header_list m_headers_response;
				// cookie
				network::http::cookie m_cookie;
				// ��Ӧ
				newobj::buffer m_response;
				// ����
				newobj::buffer m_request;
				// ��ʱ
				uint32 m_timeout;
				// ״̬��
				int32 m_status;
				// ����IP
				nstring m_proxy;
				// ����˿�
				uint32 m_proxy_port;
				// ��������
                http::method m_method;
				// �����ַ
				nstring m_url;
				// ����
				client_cache* m_cache;
				// [�ص�] ������
				std::function<void(void * data,uint32 downsize, newobj::network::http::client& client)> m_download_callback;
				std::function<void(newobj::network::http::client& client)> m_download_callback_end;
				std::function<void(newobj::network::http::client& client)> m_download_callback_failed;
				std::function<int(newobj::network::http::client& client, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) > m_download_callback_progress;
			private:
				// QT����
				bool m_qt_prog;
			};
		}

	}
}
#endif
#endif