#include "http_interceptor.h"
#if USE_NET_HTTP_WEBSITE
network::http::interceptor::interceptor()
{

}
network::http::interceptor::~interceptor()
{

}
bool network::http::interceptor::add(const nstring& path, std::function<bool(network::http::reqpack* rp)> callback)
{
	return m_values.set(path,callback,true);
}
bool network::http::interceptor::remove(const nstring& path)
{
	return m_values.del(path);
}
bool network::http::interceptor::trigger(const nstring& url, network::http::reqpack* rp)
{
	t_ret_t(m_values.size() == 0);
	m_values.lock();
	auto stdmap = m_values.parent();
	for_iter(iter, (*stdmap))
	{
		if (url.left(iter->first.length()) == iter->first)
		{
			auto callback = iter->second;
			m_values.unlock();
			return callback(rp);
		}
	}
	m_values.unlock();
	return true;
}
#endif