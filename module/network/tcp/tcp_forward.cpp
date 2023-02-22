#include "tcp_forward.h"
#ifdef USE_NET_TCP_FORWARD
newobj::network::tcp::forward::server::server():tcp::server(network::TTGen)
{
	::network::tcp::server::on_accept([](newobj::network::iserver* pServer, uint64 connID) {
		
		auto server = (network::tcp::forward::server*)pServer;

		server->m_runtime_data.mutex.lock();
		server->m_runtime_data.conn_count++;
		server->m_runtime_data.mutex.unlock();

		if (server->m_callback_onaccept == NULL)
		{
			pServer->disconn(connID);
			return;
		}
		nstring ipaddress;
		uint32 port = 0;
		if (!server->m_callback_onaccept(connID, ipaddress, port))
		{
			pServer->disconn(connID);
			return;
		}
		uint64 newConnID;

		if (server->m_agent.connect(ipaddress.c_str(), port, 5000, newConnID, (ptr)connID))
		{
			((ITcpServer*)pServer->hp())->SetConnectionExtra((CONNID)connID, (PVOID)newConnID);
		}
	});
	on_close([](newobj::network::iserver* pServer, uint64 connID) {
		std::cout << "on_close :" << connID << std::endl;
		auto server = (network::tcp::forward::server*)pServer;
		server->m_runtime_data.mutex.lock();
		server->m_runtime_data.conn_count--;
		server->m_runtime_data.mutex.unlock();
		
		PVOID extra = 0;
		if (((ITcpServer*)pServer->hp())->GetConnectionExtra((CONNID)connID, &extra))
		{
			server->m_agent.disconn((CONNID)extra);
		}
	});
	on_recv([&](newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len) {
		auto server = (network::tcp::forward::server*)pServer;
		server->m_runtime_data.mutex.lock();
		server->m_runtime_data.download_byte_size++;
		server->m_runtime_data.mutex.unlock(); 

		PVOID extra = 0;
		if (!((ITcpServer*)pServer->hp())->GetConnectionExtra((CONNID)connID, &extra))
		{
			server->m_agent.disconn((CONNID)extra);
			pServer->disconn(connID);
		}
		if (m_callback_server_recv == nullptr)
		{
			server->m_agent.send(buf, buf_len, (uint64)extra);
		}
		else
		{
			newobj::buffer data;
			data.append(buf,buf_len);
			m_callback_server_recv((uint64)extra,data);
			server->m_agent.send(data.data(), data.length(), (uint64)extra);
		}
		
	});

	m_agent.on_accept([](newobj::network::tcp::agent* agent, const uint64& connid) {

	});
	m_agent.on_close([&](newobj::network::tcp::agent* agent, const uint64& connid) {
		ptr extra = agent->extra(connid);
		if (extra == 0)
			return;
		this->disconn((uint64)extra);
	});
	m_agent.on_recv([&](newobj::network::tcp::agent* agent, const uint64& connid,const BYTE* data, uint32 length) {

		this->m_runtime_data.mutex.lock();
		this->m_runtime_data.download_byte_size+=length;
		this->m_runtime_data.upload_byte_size += length;
		this->m_runtime_data.mutex.unlock();

//		return;
		//return;
		//std::cout << "[Agent] OnReceive" << std::endl;
		ptr extra = agent->extra(connid);
		if (extra == 0)
		{
			agent->disconn(connid);
			return;
		}



		if (((ITcpServer*)this->hp())->Send((CONNID)connid, data,length) == false)
		{
			//agent->disconn(connid);
			//((ITcpServer*)this->hp())->Disconnect(connid);
		}
	});

	m_agent.m_server = this;

	if (!m_agent.start("0.0.0.0",false))
	{
		trw_str("agent start failed");
	}
}
bool newobj::network::tcp::forward::server::start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode, ushort sport_policy)
{
	if (!newobj::network::tcp::server::start(bind_ipaddress, bind_port, sportMode, sport_policy))
		return false;
	

	
	return true;
}
newobj::network::tcp::forward::server::~server()
{
	m_agent.close();
	close();
}
void newobj::network::tcp::forward::server::on_accept(std::function<bool(uint64 connid, nstring& remote_addr, uint32& remote_port)> callback)
{
	m_callback_onaccept = callback;
}

void newobj::network::tcp::forward::server::on_server_recv(std::function<void(uint64 connid, newobj::buffer& data)> callback)
{
	m_callback_server_recv = callback;
}
#endif