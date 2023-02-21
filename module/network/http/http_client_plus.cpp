
#include "http_client_plus.h"
#if USE_NET_HTTP_CLIENT
#include "http_reqpack.h"
#include "hpsocket/hpsocket.h"
//#include "hpsocket/windows/include//HPSocket-SSL.h"
#include "util/system.h"
#include "util/time.h"
#include "util/timeout.h"
#include "util/map.hpp"
#include "util/codec.h"
//#include "http_server.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
//#define _DEBUG_CLIENT

#define CLIENT ((IHttpClient*)client()) 
class http_client_listener :public IHttpClientListener
{
public:
	http_client_listener()
	{
		m_client = nullptr;
		m_status = 0;
		m_download_callback = nullptr;
		m_download_callback_end = nullptr;
		m_download_callback_failed = nullptr;
		m_content_length = 0;
		m_recv_body_length = 0;
		transfer_encoding_length = -1;
	}
	// 通过 IHttpClientListener 继承
	virtual EnHttpParseResult OnMessageBegin(IHttpClient* pSender, CONNID dwConnID) override
	{
		
#ifdef _DEBUG_CLIENT
		std::cout << "[OnMessageBegin]:\t" << dwConnID << std::endl;
#endif
		transfer_encoding_length = -1;
		m_response_body.clear();
		m_recv_body_length = 0;
		m_status = 0;
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnRequestLine(IHttpClient* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnRequestLine]:\t" << dwConnID<<"\t"<<lpszMethod<<"\t"<<lpszUrl << std::endl;
#endif
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnStatusLine(IHttpClient* pSender, CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnStatusLine]:\t" << dwConnID << "\t" << usStatusCode << "\t" << lpszDesc << std::endl;
#endif
		m_status = usStatusCode;
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnHeader(IHttpClient* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnHeader]:\t" << dwConnID << "\t" << lpszName << ": " << lpszValue << std::endl;
#endif
		if (strcmp("Content-Length", lpszName) == 0)
		{
			m_content_length = nstring(lpszValue).to_uint64();
		}
		m_headers_response.set(lpszName,lpszValue);
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnHeadersComplete(IHttpClient* pSender, CONNID dwConnID) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnHeadersComplete]"<< std::endl;
#endif
		return EnHttpParseResult::HPR_OK;
	}
	std::string dec2hex(int i) //将int转成16进制字符串
	{
		std::stringstream ioss; //定义字符串流
		std::string s_temp; //存放转化后字符
		ioss << std::setiosflags(std::ios::uppercase) << std::hex << i; //以十六制(大写)形式输出
		//ioss << resetiosflags(ios::uppercase) << hex << i; //以十六制(小写)形式输出//取消大写的设置
		ioss >> s_temp;
		return s_temp;
	}
	virtual EnHttpParseResult OnBody(IHttpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnBody]:\t"<< iLength << std::endl;
#endif
		if (m_download_callback != nullptr)
		{
			if (m_download_callback((void*)pData, iLength, m_content_length, *m_client))
				return EnHttpParseResult::HPR_OK;
			else
				return EnHttpParseResult::HPR_ERROR;
		}
		else
		{
			//if (transfer_encoding_length != -1)
			//{
			//	nstring length = dec2hex(iLength) + "\r\n";
			//	m_response_body.append(length);
			//}
			m_response_body.append(pData, iLength);
		}
			
		
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnChunkHeader(IHttpClient* pSender, CONNID dwConnID, int iLength) override
	{
		transfer_encoding_length = iLength;
#ifdef _DEBUG_CLIENT
		std::cout << "[OnChunkHeader]:\t" << dwConnID<<"\t"<<iLength << std::endl;
#endif
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnChunkComplete(IHttpClient* pSender, CONNID dwConnID) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnChunkComplete]:\t" << dwConnID << std::endl;
#endif
		//m_response_body.append("\r\n0\r\n\r\n\r\n",9);
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnMessageComplete(IHttpClient* pSender, CONNID dwConnID) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnMessageComplete]:\t" << dwConnID << std::endl;
#endif
		if (m_download_callback_end!= nullptr)
			m_download_callback_end(*m_client);
		m_recv_state = 1;
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnUpgrade(IHttpClient* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnUpgrade]:\t" << dwConnID << std::endl;
#endif
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHttpParseResult OnParseError(IHttpClient* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnParseError]:\t" << dwConnID << std::endl;
#endif
		if (m_download_callback_failed != nullptr)
			m_download_callback_failed(*m_client);

		m_recv_state = 2;
		return EnHttpParseResult::HPR_OK;
	}
	virtual EnHandleResult OnWSMessageHeader(IHttpClient* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnWSMessageHeader]:\t" << dwConnID << std::endl;
#endif
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnWSMessageBody(IHttpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnWSMessageBody]:\t" << dwConnID << std::endl;
#endif
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnWSMessageComplete(IHttpClient* pSender, CONNID dwConnID) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnWSMessageComplete]:\t" << dwConnID << std::endl;
#endif
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnHandShake]:\t" << dwConnID << std::endl;
#endif
		m_connect_state = 2;
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		if(m_client->m_close == true)
			return EnHandleResult::HR_ERROR;
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		if (m_client->m_close == true)
			return EnHandleResult::HR_ERROR;
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) override
	{
		if (m_client->m_close == true)
			return EnHandleResult::HR_ERROR;
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnClose]:\t" << dwConnID << std::endl;
#endif
		m_recv_state = 2;
		m_connect_state = 0;
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnPrepareConnect]:\t" << dwConnID << std::endl;
#endif
		return EnHandleResult::HR_OK;
	}
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override
	{
#ifdef _DEBUG_CLIENT
		std::cout << "[OnConnect]:\t" << dwConnID << std::endl;
#endif
		return EnHandleResult::HR_OK;
	}
public:
	int32 transfer_encoding_length;
	// 预计接收长度
	uint64 m_content_length;
	// 已接收长度
	uint64 m_recv_body_length;
	// 客户端
	network::http::client_plus *m_client;
	// 0=已断开 1=连接中 2=已连接
	int m_connect_state;
	// 0=请求中 1=接收成功 2=请求失败
	int m_recv_state;
	// 返回数据
	newobj::buffer m_response_body;
	// [header] 响应
	network::http::header_list m_headers_response;
	// 状态码
	ushort m_status;
	std::function<bool(void* data, uint32 downsize,uint64 allsize,newobj::network::http::client_plus& client)> m_download_callback;
	std::function<void(newobj::network::http::client_plus& client)> m_download_callback_end;
	std::function<void(newobj::network::http::client_plus& client)> m_download_callback_failed;
};
void newobj::network::http::client_plus::close()
{
	m_close = true;
}
newobj::network::http::client_plus::client_plus()
{
	m_listener = new http_client_listener;
#ifdef LIB_HPSOCKET_SSL
	m_client_ssl = HP_Create_HttpsClient(m_listener);
#else
	m_client_ssl = nullptr;
#endif
	m_client = HP_Create_HttpClient(m_listener);
	m_listener->m_client = this;
	m_init = false;
	m_port = 0;
	m_ssl = false;
	m_timeout_connect_msec = 3000;
	m_timeout_recv_msec = 8000;
	m_cache = nullptr;
}
newobj::network::http::client_plus::~client_plus()
{
	

#ifdef LIB_HPSOCKET_SSL
	((IHttpClient*)m_client_ssl)->Stop();
	((IHttpClient*)m_client_ssl)->Wait();
	HP_Destroy_HttpsClient(((IHttpClient*)m_client_ssl));
#endif
	((IHttpClient*)m_client)->Stop();
	((IHttpClient*)m_client)->Wait();
	HP_Destroy_HttpClient(((IHttpClient*)m_client));

    delete m_listener;
}
void newobj::network::http::client_plus::set_timeout(uint32 connect_msec, uint32 recv_msec)
{
	m_timeout_connect_msec = connect_msec;
	m_timeout_recv_msec = recv_msec;
}
bool newobj::network::http::client_plus::get(const nstring& url, const std::map<nstring, nstring>& value)
{
	m_method = network::http::GET;

	nstring param_str;
	for_iter(iter, value)
		param_str += (param_str.length() == 0 ? "" : "&") + iter->first + "=" + iter->second;

	f_ret_f(init());
	f_ret_f(parseurl(nstring(url + (param_str.length() == 0 ? "" : "?") + param_str)));
	m_url = url;
	f_ret_f(connect());
	return request();
}
bool newobj::network::http::client_plus::post(const nstring& url, const std::map<nstring, nstring>& value,bool to_utf8)
{
	nstring param_str;
	for_iter(iter, value)
		param_str += (param_str.length() == 0 ? "" : "&") + iter->first + "=" + iter->second;
	if (to_utf8)
	{
		param_str = codec::to_utf8(param_str);
		m_headers_request.set("Content-Type", "application/x-www-form-urlencoded;charset=utf-8");
	}
	else
	{
		m_headers_request.set("Content-Type", "application/x-www-form-urlencoded");
	}
	m_request_body = param_str;
	return post(url);
}
bool newobj::network::http::client_plus::post(const nstring& url, const newobj::json& value, bool to_utf8)
{
	m_headers_request.set("Content-Type", "application/json");
	if (to_utf8)
		return post(url, newobj::buffer(codec::to_utf8(value.to_string())));
	else
		return post(url, newobj::buffer(value.to_string()));
}
bool newobj::network::http::client_plus::post(const nstring& url, const newobj::buffer& value)
{
	m_request_body = value;
	return post(url);
}
bool newobj::network::http::client_plus::post(const nstring& url, const http::make_form& value)
{
	nstring boundary;
	value.make(m_request_body, boundary);
	m_headers_request.set("Content-Type", "boundary=" + boundary + "; multipart/form-data");
	return post(url);
}
network::http::header_list& newobj::network::http::client_plus::headers_request()
{
	// TODO: 在此处插入 return 语句
	return m_headers_request;
}
network::http::header_list& newobj::network::http::client_plus::headers_response()
{
	// TODO: 在此处插入 return 语句
	return m_listener->m_headers_response;
}
uint32 newobj::network::http::client_plus::status()
{
	return m_listener->m_status;
}
newobj::buffer& newobj::network::http::client_plus::response()
{
	return m_listener->m_response_body;
}
void newobj::network::http::client_plus::cache(client_cache* cache)
{
	m_cache = cache;
}
bool newobj::network::http::client_plus::parseurl(const nstring& url)
{
	if (url.left(7) == "http://")
		m_ssl = false;
	else if (url.left(8) == "https://")
		m_ssl = true;
	else
	{
		m_lastErrorDesc = "Only https and http protocols are supported,Bad address:"+std::string(url);
		return  false;
	}
	nstring pu;
	if (m_ssl)
		pu = url.right(url.length() - 8);
	else
		pu = url.right(url.length() - 7);
	size_t index = pu.find("/");
	if (index != -1)
	{
		m_path = pu.right(pu.length() - index);
		auto arr = pu.split('/');
		if (arr.size() < 2)
		{
			m_lastErrorDesc = "parseurl failed 0x01";
			return false;
		}
		pu = arr[0];
	}
	else
		m_path = "/";
	// www.baidu.com:443

	index = pu.find(":");
	if (index == -1)
	{
		if (m_ssl)
			m_port = 443;
		else
			m_port = 80;
	}
	else
	{
		auto arr = pu.split(':');
		if (arr.size() != 2)
		{
			m_lastErrorDesc = "parseurl failed 0x02";
			return false;
		}
		pu = arr[0];
		m_port = arr[1].to_ushort();
	}
	m_ipaddress = pu;// network::to_ip(pu);
#ifndef LIB_HPSOCKET_SSL
	if (m_ssl)
	{
		std::cout << "The hpsocket does not support the https protocol. Please define a LIB_ HPSOCKET_ SSL Macro" << std::endl;
	}
#endif
	
	return true;
}
bool newobj::network::http::client_plus::connect()
{
	m_listener->m_connect_state = 1;
	if (CLIENT->Start(m_ipaddress.c_str(), m_port) == false)
	{
		m_lastErrorDesc = nstring("start failed," + nstring::from((uint64)SYS_GetLastError()));
		return false;
	}
	timestamp start_msec = time::now_msec();
	while (m_listener->m_connect_state == 1)
	{
		system::sleep_msec(10);
		if (start_msec + m_timeout_connect_msec < time::now_msec())
		{
			m_lastErrorDesc = "connection timed out";
			return false;
		}
	}
		
	return m_listener->m_connect_state == 2;
}
bool newobj::network::http::client_plus::init()
{
	((IHttpClient*)m_client)->Stop();
	((IHttpClient*)m_client)->Wait();
#ifdef LIB_HPSOCKET_SSL
	((IHttpClient*)m_client_ssl)->Stop();
	((IHttpClient*)m_client_ssl)->Wait();
	((IHttpClient*)m_client_ssl)->CleanupSSLContext();
	if (((IHttpClient*)m_client_ssl)->SetupSSLContext() == false)
	{
		m_lastErrorDesc = nstring("SetupSSLContext failed," + nstring::from((uint64)SYS_GetLastError())).c_str();
		return false;
	}
#endif
	return true;
}
bool newobj::network::http::client_plus::request()
{
//	std::cout << CLIENT->GetSSLCipherList() << std::endl;
	m_close = false;
	m_listener->m_recv_state = 0;
	const char* method;
	switch (m_method)
	{
	case newobj::network::http::GET:
		method = "GET";
		{
			/*【缓存】置头*/
			if (m_cache != nullptr && m_method == GET)
			{
				m_cache->set_header(this, m_url);
			}
		}
		break;
	case newobj::network::http::POST:
		method = "POST";
		break;
	case newobj::network::http::PUT:
		method = "PUT";
		break;
	case newobj::network::http::DEL:
		method = "DEL";
		break;
	default:
		m_lastErrorDesc = "Unsupported request type";
		return false;
		break;
	}
	/*置Cookie*/
	{
		if (m_cookie.to_string() != "")
		{
			std::cout << m_cookie.to_string().c_str() << std::endl;
			m_headers_request.set("Cookie", m_cookie.to_string());
		}
			
	}
	/*完善协议头*/
	{
		if(m_headers_request.exist("User-Agent") == false)
			m_headers_request.set("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:108.0) Gecko/20100101 Firefox/108.0");
		if (m_headers_request.exist("Accept") == false)
			m_headers_request.set("Accept", "*/*");
		if (m_headers_request.exist("Accept-Language") == false)
			m_headers_request.set("Accept-Language", "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
		if (m_headers_request.exist("Connection") == false)
			m_headers_request.set("Connection", "keep-alive");
	}

	THeader* pHeader = nullptr;
	auto header_map = m_headers_request.to();
	size_t nHeader = header_map.size();
	if (nHeader != 0)
		pHeader = new THeader[nHeader];
	size_t count = 0;
	for_iter(iter,header_map)
	{
		pHeader[count].name = iter->first.c_str();
		pHeader[count].value = iter->second.c_str();
		count++;
	}
	

	bool result =  CLIENT->SendRequest(
		method,
		m_path.c_str(),
		pHeader,
		nHeader,
		(const BYTE*)m_request_body.data(),
		m_request_body.length()
	);
	delete[] pHeader;
	if (result == false)
	{
		uint64 error = SYS_GetLastError();
		m_lastErrorDesc = nstring("request failed," + nstring::from((uint64)error));
		return false;
	}

	timestamp start_msec = time::now_msec();
	while (m_listener->m_recv_state == 0)
	{
		system::sleep_msec(10);
		if (start_msec + m_timeout_recv_msec < time::now_msec())
		{
			m_lastErrorDesc = "recv timed out";
			return false;
		}
	}
	result =  m_listener->m_recv_state == 1;
	f_ret_f(result);
	/*【缓存】判断缓存*/
	if (m_cache != nullptr && m_method == GET)
	{
		bool cache = false;
		if (this->status() == 304)
		{
			cache = m_cache->read(this, m_listener->m_response_body);
		}
		if (cache == false)
		{
			m_cache->write(this);
		}
	}
	/*合并响应cookie*/
	{
		if (m_listener->m_headers_response.exist("Set-Cookie") != false && m_listener->m_headers_response.get("Set-Cookie").to_string() != "")
			m_cookie.merge(m_listener->m_headers_response.get("Set-Cookie").to_string());
	}
	return !m_close;
}

bool newobj::network::http::client_plus::post(const nstring& url)
{
	m_method = network::http::POST;
	f_ret_f(init());
	f_ret_f(parseurl(url));
	m_url = url;
	f_ret_f(connect());
	return request();
}
void* newobj::network::http::client_plus::client()
{
	if (m_ssl)
	{
#ifdef LIB_HPSOCKET_SSL
		return m_client_ssl;
#else
		return m_client;
#endif
	}
	else
		return m_client;
}
void newobj::network::http::client_plus::on_down_ing(const std::function<bool(void* data, uint32 downsize, uint64 allsize, newobj::network::http::client_plus& client)>& callback)
{ m_listener->m_download_callback = callback; }
void newobj::network::http::client_plus::on_down_end(const std::function<void(newobj::network::http::client_plus& client)>& callback)
{ m_listener->m_download_callback_end = callback; }
void newobj::network::http::client_plus::on_down_failed(const std::function<void(newobj::network::http::client_plus& client)>& callback)
{ m_listener->m_download_callback_failed = callback; }
#endif
