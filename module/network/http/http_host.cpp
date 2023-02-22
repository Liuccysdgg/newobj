
#include "http_host.h"
#if USE_NET_HTTP_WEBSITE
newobj::network::http::host::host()
{
	m_port = 0;

}

newobj::network::http::host::~host()
{
}

void newobj::network::http::host::init(const nstring& host,ushort port, network::http::ssl* ssl)
{
	m_host = host;
	m_port = port;
	m_ssl = ssl;
	if (m_port == 443 || m_port == 80)
		m_return_host = host;
	else
		m_return_host = host + ":" + nstring::from(m_port);
}
#endif