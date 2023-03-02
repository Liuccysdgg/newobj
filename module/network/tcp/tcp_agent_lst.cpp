
#include "tcp_agent_lst.h"
#ifdef USE_NET_TCP_AGENT
#include "public/exception.h"
#include "tcp_agent.h"
#include "tcp_forward.h"
struct AutoFreeAgentConnID_
{
	uint64 connid;
	newobj::map<uint64, int>* map;
};
newobj::network::tcp::tcp_agent_lst::tcp_agent_lst()
{
    this->m_agent = nullptr;
}
newobj::network::tcp::tcp_agent_lst::~tcp_agent_lst()
{
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnPrepareConnect(ITcpAgent* pSender, CONNID dwConnID, SOCKET socket)
{
    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnSend(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnReceive(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	if (m_agent->m_pfun_onrecv != nullptr)
		m_agent->m_pfun_onrecv(m_agent,dwConnID,pData,iLength);
	return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnClose(ITcpAgent* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	/*失败*/
	m_map.set(dwConnID, 2,true);

	if (m_agent->m_pfun_onclose != nullptr)
		m_agent->m_pfun_onclose(m_agent, dwConnID);

	
	auto param = new AutoFreeAgentConnID_;
	param->connid = dwConnID;
	param->map = &m_map;

	m_timeout.add([](
		timestamp create/*创建时间*/,
		uint32 & timeout_msec/*过期时间[毫秒]*/,
		nstring id/*超时事件ID*/,
		ptr extra_data/*附加数据*/)->TimeOutRet {
			AutoFreeAgentConnID_* conf = (AutoFreeAgentConnID_*)extra_data;
			conf->map->del(conf->connid);
			delete conf;
			return TOR_CLOSE;
		}, 3000, (ptr)param);

    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnShutdown(ITcpAgent* pSender)
{
	return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnReceive(ITcpAgent* pSender, CONNID dwConnID, int iLength)
{
    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnConnect(ITcpAgent* pSender, CONNID dwConnID)
{
	/*成功*/
	m_map.set(dwConnID, 1, true);
	if (m_agent->m_pfun_onaccept != nullptr)
		m_agent->m_pfun_onaccept(m_agent, dwConnID);
	
    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_agent_lst::OnHandShake(ITcpAgent* pSender, CONNID dwConnID)
{
	return HR_OK;
}
#endif