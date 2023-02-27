#include "http_ssl.h"
#if USE_NET_HTTP_WEBSITE
#include "http_server.h"
#include "util/file.h"
#include "hpsocket/hpsocket.h"
#ifdef LIB_HPSOCKET_SSL
#define HPSERVER ((IHttpServer*)m_server->hpserver())
newobj::network::http::ssl::ssl(network::http::server* server,ssl_verify_type verify_type, const nstring& pem_cert, const nstring& pem_key, const nstring& pem_ca, const nstring& password)
{
	m_verify_type = verify_type;
	m_pem_cert =  newobj::file::read(pem_cert).to_string();
	m_pem_key = newobj::file::read(pem_key).to_string();
	m_pem_ca = newobj::file::read(pem_ca).to_string();
	//m_pem_cert =  pem_cert;
	//m_pem_key = pem_key;

	m_pem_password = password;
	m_server = server;
	m_index = -1;
}

newobj::network::http::ssl::~ssl()
{
}
bool newobj::network::http::ssl::bind(const nstring& host)
{
	return HPSERVER->BindSSLServerName(host.c_str(),m_index);
}
bool newobj::network::http::ssl::regist()
{
    if(m_server->m_init_ssl == false){
        m_server->m_init_ssl = true;
        if (HPSERVER->SetupSSLContextByMemory(
                                              (EnSSLVerifyMode)m_verify_type,
                                              m_pem_cert.c_str(),
                                              m_pem_key.c_str(),
                                              m_pem_password.c_str(),
											  m_pem_key.c_str()) == false)
        {
            m_lastErrorDesc = "SetupSSLContextByMemory Failed, code:"+nstring::from((uint64)SYS_GetLastError());
            return false;
        }

    }
	m_index = HPSERVER->AddSSLContextByMemory((EnSSLVerifyMode)m_verify_type,
		m_pem_cert.c_str(),
		m_pem_key.c_str(),
		m_pem_password.c_str());
	if(m_index < 0)
	{
		m_lastErrorDesc = "AddSSLContextByMemory Failed, code:" + nstring::from((uint64)SYS_GetLastError());
		return false;
	}
	return true;
}
#endif
#endif
