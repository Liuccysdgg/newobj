#include "udp_node.h"
#if USE_NET_UDP_NODE
#include "public/exception.h"
#include "hpsocket/hpsocket.h"
#include "udp_node_lst.h"

namespace newobj
{
	namespace network
	{
		namespace udp
		{
			node::node()
			{
				m_pfun_onrecv = NULL;
			}
			node::~node()
			{
				close();
			}
			bool node::start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode, ushort sport_policy)
			{
				m_sportMode = sportMode;
				m_sport_policy = sport_policy;
				m_bind_ipaddress = bind_ipaddress;
				m_bind_port = bind_port;
				//判断是否已存在服务端
				if (this->m_hp_ptr != 0)
				{
					this->m_lastErrorDesc = "node started";
					return false;
				}
				m_hp_listener_ptr = (ptr)new udp_node_lst(this);
				m_hp_ptr = (ptr)HP_Create_UdpNode((udp_node_lst*)m_hp_listener_ptr);

				if (to_ptr(IUdpNode*, m_hp_ptr)->Start(m_bind_ipaddress.c_str(), m_bind_port) == false)
				{
					this->m_lastErrorDesc = "node start failed, error code:" + nstring::from(to_ptr(IUdpNode*, m_hp_ptr)->GetLastError());
					HP_Destroy_UdpNode((IUdpNode*)m_hp_ptr);
					delete to_ptr(udp_node_lst*, m_hp_listener_ptr);
					m_hp_ptr = 0;
					return false;
				}
				return true;
			}
			bool node::close(uint32 wait_msec)
			{
				if (this->m_hp_ptr == 0)
					return false;

				if (to_ptr(IUdpNode*, this->m_hp_ptr)->Stop() == false)
				{
					this->m_lastErrorDesc = "node stop failed, error code:" + nstring::from((uint32)SYS_GetLastError());
					return false;
				}
				if (to_ptr(IUdpNode*, this->m_hp_ptr)->Wait(wait_msec == 0 ? -1 : wait_msec) == false)
				{
					this->m_lastErrorDesc = "node waiting stop failed, error code:" + nstring::from((uint32)SYS_GetLastError());
					return false;
				}

				HP_Destroy_UdpNode(to_ptr(IUdpNode*, this->m_hp_ptr));
				this->m_hp_ptr = 0;
				delete (udp::udp_node_lst*)this->m_hp_listener_ptr;
				return true;
			}
			bool node::send(const nstring& remote_ipaddress, ushort remote_port, const char* pData, uint32 dataLen)
			{
				if (this->m_hp_ptr == 0)
					return false;
				return to_ptr(IUdpNode*, m_hp_ptr)->Send(remote_ipaddress.c_str(), remote_port, (const BYTE*)pData, dataLen);
			}

			void node::on_recv(std::function<void(node* node, const nstring& remote_ipaddress, ushort remote_port, const char* buf, uint32 buf_len)> pfun)
			{
				this->m_pfun_onrecv = pfun;
			}

		}
		
	}
}
#endif