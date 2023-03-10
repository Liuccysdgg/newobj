#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <string>
#include "hpsocket/hpsocket.h"
#include <vector>
#include "util/forstring.h"

#include "http_session.h"
#include "util/json.h"
#include "http_parser.h"
#include "http_interface.h"
namespace newobj
{
	namespace network
	{
		namespace http
		{
            class reqpack;
			class server; 
			/********************************************************************
             * class：Http请求解析类
			 ********************************************************************/
            class MNETWORK_API request:public network::http::interface_
            {
			public:
				request();
				~request();

                /***************************************************************************
                 * function：取协议头
                 * param
                 *		name                        ：               名称
                 *      value                       ：               内容
                 ***************************************************************************/
                bool header(const nstring& name, nstring& value);

                /***************************************************************************
                 * function：取请求类型
                 ***************************************************************************/
                network::http::method method();
                /***************************************************************************
                 * function：取请求路径
                 ***************************************************************************/
                nstring filepath();
                /***************************************************************************
                 * function：取请求主机
                 ***************************************************************************/
                nstring host();
                /***************************************************************************
                 * function：取Cookie
                 ***************************************************************************/
                bool cookie(const nstring& name, nstring& value);
                /***************************************************************************
                 * function：取Session
                 ***************************************************************************/
                network::http::session* session(const nstring& name);
                /***************************************************************************
                 * function：取reqpack
                 ***************************************************************************/
                network::http::reqpack* reqpack();
                /***************************************************************************
                 * function：解析器
                 ***************************************************************************/
                network::http::parser* parser();
                /***************************************************************************
                 * function：Get Browserr Remote Ipaddress
                 ***************************************************************************/
                nstring remote_ipaddress(bool find_header = false,const nstring& inside_ipaddress = "");
                friend class reqpack;
			private:
                network::http::reqpack *m_reqpack;
                network::http::session* m_session;
                network::http::parser m_parser;
			};
		}
	}
}
#endif
