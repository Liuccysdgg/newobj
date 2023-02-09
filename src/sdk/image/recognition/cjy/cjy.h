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
			/*超级鹰验证码识别*/
			class cjy
			{
			public:
				cjy();
				~cjy();
				/*置用户信息*/
				void set_user(const nstring& username,const nstring& password);
				/*置域名*/
				void set_domain(const nstring& domain);
				/*查询题分*/
				bool score(uint32& value);
				
			public:
				/*【仅调试】注册*/
				static nstring regist(const nstring& domain,const nstring& username, const nstring& password);
				/*识别*/
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