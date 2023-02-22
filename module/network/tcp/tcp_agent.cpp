#include "tcp_agent.h"
#ifdef USE_NET_TCP_AGENT
#include "util/time.h"
#include "util/system.h"
#include "tcp_forward.h"
newobj::network::tcp::agent::agent()
{
	m_agent = nullptr;
	m_listener = nullptr;
	m_pfun_onaccept = nullptr;
	m_pfun_onclose = nullptr;
	m_pfun_onrecv = nullptr;
	m_server = nullptr;
}

newobj::network::tcp::agent::~agent()
{

	if (m_agent != nullptr)
	{
		HP_Destroy_TcpAgent(m_agent);
		delete m_listener;
		m_agent = nullptr;
		m_listener = nullptr;
	}
}



bool newobj::network::tcp::agent::start(const nstring& address, bool async)
{
	if (m_agent == nullptr)
	{
		m_listener = new network::tcp::tcp_agent_lst;
		m_agent = HP_Create_TcpAgent(m_listener);
		m_agent->SetSendPolicy(SP_DIRECT);
		m_listener->m_agent = this;

	}
	return m_agent->Start(address.c_str(),async);
}

bool newobj::network::tcp::agent::disconn(uint64 connid)
{
	

	return m_agent->Disconnect((CONNID)connid);
}

bool newobj::network::tcp::agent::disconn_longconnections(uint32 msec)
{
	return m_agent->DisconnectLongConnections(30, 1);
}

bool newobj::network::tcp::agent::close(uint32 wait_msec)
{
	if (m_agent != nullptr)
	{
		f_ret_f(m_agent->Stop());
		return m_agent->Wait(wait_msec);
	}
	return true;
}

bool newobj::network::tcp::agent::connect(const nstring& address, uint32 port, int32 wait_msec, uint64& connid, ptr extra)
{
	CONNID connid_hp = 0;
	f_ret_f(m_agent->Connect(address.c_str(), port, &connid_hp,0));
	connid = connid_hp;
	timestamp start_time = time::now_msec();
	m_listener->m_map.set(connid_hp,0,true);
	while (m_listener->m_map[connid_hp] == 0)
	{
		if (wait_msec != -1)
		{
			if (time::now_msec() - start_time > wait_msec)
				break;
		}
		newobj::system::sleep_msec(10);
	}
	if (m_listener->m_map[connid_hp] == 2)
	{
		m_listener->m_map.del(connid_hp);
		return false;
	}
	m_agent->SetConnectionExtra(connid_hp,(PVOID)extra);
	return true;
}

bool newobj::network::tcp::agent::conned(uint64 connid)
{
	return m_agent->IsConnected((CONNID)connid);
}
bool newobj::network::tcp::agent::send(const char* buff, uint32 len,uint64 connid)
{
	if (m_server != nullptr)
	{
		m_server->m_runtime_data.mutex.lock();
		m_server->m_runtime_data.upload_byte_size += len;
		m_server->m_runtime_data.mutex.unlock();
	}
	

	return m_agent->Send((CONNID)connid,(const BYTE*)buff,len);
}

bool newobj::network::tcp::agent::send(const buffer& data,uint64 connid)
{
	return send(data.data(),data.length(), connid);
}

bool newobj::network::tcp::agent::extra(uint64 connid, ptr data)
{
	m_agent->SetConnectionExtra((CONNID)connid, (PVOID)data);
	return false;
}

ptr newobj::network::tcp::agent::extra(uint64 connid)
{
	PVOID extra = 0;
	if (m_agent->GetConnectionExtra((CONNID)connid, &extra))
	{
		return (ptr)extra;
	}
	return 0;
}

void newobj::network::tcp::agent::on_recv(std::function<void(network::tcp::agent*, const uint64& connid, const BYTE* data, uint32 length)> cb)
{
	m_pfun_onrecv = cb;
}

void newobj::network::tcp::agent::on_accept(std::function<void(network::tcp::agent*,const uint64& connid)> cb)
{
	m_pfun_onaccept = cb;
}

void newobj::network::tcp::agent::on_close(std::function<void(network::tcp::agent*, const uint64& connid)> cb)
{
	m_pfun_onclose = cb;
}
#endif