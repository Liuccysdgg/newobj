#pragma once
#include "public/define.h"
#include "public/string.h"
#include "network/http/http_client_plus.h"

namespace newobj
{
	namespace image
	{
		namespace recognition
		{
			/*����ӥ��֤��ʶ��*/
			class cjy
			{
			public:
				cjy();
				~cjy();
				/*���û���Ϣ*/
				void set_user(const nstring& username,const nstring& password);
				/*������*/
				void set_domain(const nstring& domain);
				/*��ѯ���*/
				bool score(uint32& value);
				
			public:
				/*�������ԡ�ע��*/
				static nstring regist(const nstring& domain,const nstring& username, const nstring& password);
				/*ʶ��*/
				static bool detect(const nstring& domain, const nstring& username, const nstring& password_md5,uint32 softid,nstring codetype,const newobj::buffer& image, nstring& result, nstring& error_msg);
			private:
				nstring m_username;
				nstring m_password;
				nstring m_domain;
				network::http::client_plus m_client;
			};
		}
	}
}