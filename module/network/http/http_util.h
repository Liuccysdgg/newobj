#pragma once
#include "http_define.h"
#if USE_NET_HTTP_UTIL
#include <string>
#include "public/define.h"

#include "util/forstring.h"
#include "public/buffer.h"
#include "network/tcp/tcp_server.h"
//class IHttpClient;
//class IHttpClientListener;
namespace newobj
{
    namespace network
    {
        namespace tools
        {
            /*ȡ�ĵ�����*/
            void MNETWORK_API content_type(const nstring& extName, nstring& type);
            /*���URL*/
            bool MNETWORK_API parse_url(const nstring& url, nstring& httpType, nstring& host, nstring& ipaddress, ushort& port, nstring& urlField);
            /*���URL*/
            bool MNETWORK_API parse_url_host(const nstring& url, nstring& host);
			/*INT IP ת �ַ���IP*/
			void MNETWORK_API to_string(uint32 int_ip, nstring& ipaddress);
			struct TcpConf
			{
				TcpConf()
				{
					local_port = 0;
					remote_port = 0;
					__local_ipaddress = 0;
					__remote_ipaddress = 0;
				}
				void local_ipaddress(nstring& ipaddress) const
				{
					to_string(__local_ipaddress, ipaddress);
				}
				void remote_ipaddress(nstring& ipaddress) const
				{
					to_string(__remote_ipaddress, ipaddress);
				}
				uint32 __local_ipaddress;
				uint32 __remote_ipaddress;
				uint32 local_port;
				uint32 remote_port;
			};

			/*URLתIP��ַ*/
			nstring  MNETWORK_API to_ip(const nstring& url);
			/*���TCP�˿�״̬*/
			bool MNETWORK_API tcp_status(ushort port, ulong& status);
			/*ȡTCP�б�*/
			void MNETWORK_API tcp_list(std::list<TcpConf>& list);

			/*ȡ����IP�б�*/
			std::vector<nstring> MNETWORK_API iplist();
			/*�˿��Ƿ�ռ��*/
			inline bool is_occupy(const nstring& ip, uint32 port)
			{
#ifdef LIB_HPSOCKET
				network::tcp::server server(network::TTGen);
				return !server.start(ip, port);
#else
				return false;
#endif
			}
			/*
				ȡ��С����
				B/KB/MB/GB
			*/
			nstring MNETWORK_API size_name(double size, uint32 fixe = 0);
        }
    }
        
}
#endif
