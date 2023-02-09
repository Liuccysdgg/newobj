#pragma once
#include "network/define.h"
#if USE_NET_UDP_NODE
#include "network/inet.h"
#include "public/base.h"
#include <functional>
namespace newobj
{
	namespace network
	{
		namespace udp
		{
			class node;
			class udp_node_lst;
			/*************************************************************************
			 * class£ºÆÕÍ¨UDP
			 *************************************************************************/
			class MNETWORK_API node :public inet, public base
			{
			public:
				node();
				~node();
				// Í¨¹ý IServer ¼Ì³Ð
				bool start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode = false, ushort sport_policy = 10000);
				bool close(uint32 wait_msec = 0);
				bool send(const nstring& remote_ipaddress, ushort remote_port, const char* pData, uint32 dataLen);
				 
				void on_recv(std::function<void(node* node, const nstring& remote_ipaddress, ushort remote_port, const char* buf, uint32 buf_len)> pfun);
				friend class udp_node_lst;
			private:
				std::function<void(node* node, const nstring& remote_ipaddress, ushort remote_port, const char* buf, uint32 buf_len)> m_pfun_onrecv;
			};
		}
		

	}
}
#endif