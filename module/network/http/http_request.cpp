
#include "http_request.h"
#if USE_NET_HTTP_WEBSITE
#include "util/forstring.h"
#include "util/codec.h"
#include "http_session.h"
#include "http_server.h"
#include "public/exception.h"
#include "log/log_define.h"
#include "http_reqpack.h"
#include "http_center.h"
#include "http_website.h"
#include "hpsocket/hpsocket.h"
#define HPSERVER ((IHttpServer*)m_reqpack->server()->hpserver())
namespace newobj
{
	namespace network
	{
		namespace http
		{
			request::request()
			{
                m_reqpack = nullptr;
                m_session = nullptr;
			}
			request::~request()
			{
                if (m_session != nullptr)
                    delete m_session;
            }

            bool request::header(const nstring &name, nstring &value)
            {
                
                LPCSTR lpszValue = nullptr;
                if (HPSERVER->GetHeader((CONNID)m_reqpack->connid(), name.c_str(), &lpszValue) == false)
                    return false;
                value = lpszValue;
                return true;
            }

            method request::method()
            {

                return m_reqpack->method();
            }

            nstring request::filepath()
            {
                return m_reqpack->filepath();
            }

            nstring request::host()
            {
                return m_reqpack->host();
            }

            bool request::cookie(const nstring &name, nstring &value)
            {
                LPCSTR lpszValue = nullptr;
                if (HPSERVER->GetCookie((CONNID)m_reqpack->connid(), name.c_str(), &lpszValue) == false)
                    return false;
                value = lpszValue;
                return true;
            }
            network::http::session* request::session(const nstring& name)
            {
                if (m_session == nullptr)
                {
                    m_session = new network::http::session;
                    m_session->m_mgr = m_reqpack->website()->session();
                    nstring session_name;
                    if (cookie(name, session_name) && session_name != "")
                    {
                        m_session->init(session_name);
                    }
                }
                return m_session;
            }
            network::http::reqpack* request::reqpack()
            {
                return m_reqpack;
            }
            network::http::parser* request::parser()
            {
                nstring content_type;
                {
                    header("Content-Type", content_type);
                    if (content_type == "")
                    {
                        content_type = "application/x-www-form-urlencoded";
                    }
                }
                m_parser.init(HPSERVER->GetUrlField((CONNID)m_reqpack->connid(), HUF_QUERY), m_reqpack->method(), m_reqpack->data(), content_type);
                return &m_parser;
            }

            nstring request::remote_ipaddress(bool find_header,const nstring& inside_ipaddress)
            {
                bool is_inside = false;
                nstring ipaddress;
                ushort port;
                reqpack()->server()->remote(reqpack()->connid(),ipaddress,port);
                {
                    if(ipaddress.left(2) == "10" || ipaddress.left(6) == "172.16" || ipaddress.left(7) == "192.168" || (ipaddress == inside_ipaddress && inside_ipaddress != ""))
                        is_inside = true;
                }

                if(find_header && is_inside)
                    header("X-Real-IP",ipaddress);
                return ipaddress;
            }
		}
	}
}
#endif
