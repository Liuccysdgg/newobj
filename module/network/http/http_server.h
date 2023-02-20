#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <map>
#include "public/define.h"
#include "public/base.h"
#include "util/json.h"
#include "util/lock.h"
#include "http_interface.h"
#include "network/qps.hpp"
#include "util/object_pool.hpp"
namespace newobj
{
	namespace network
	{
		namespace http
		{
            class http_server_lst;
            class host;
			/*************************************************************************
             * class��HTTP������
			 *************************************************************************/
            class MNETWORK_API server:public newobj::base,public network::http::interface_
			{
			public:
				server();
				~server();
                bool create(bool https, ushort port);
                /******************************************************************
                 * Function������
                 * Param��
                 *      ipaddress               ��           ������ַ
                 *      port                        ��           �����˿�
                 ******************************************************************/
                bool start();
                /******************************************************************
                 * Function���ر�
                 ******************************************************************/
                bool close();
                /******************************************************************
                 * Function��ȡԶ����Ϣ
                 * Param��
                 *      connid                   ��           ����ID
                 *      ipaddress                ��           Զ�̵�ַ
                 *      port                     ��           Զ�̶˿�
                 ******************************************************************/
                bool remote(uint64 connid, nstring& ipaddress, ushort& port);

                /******************************************************************
                 * Function��HP����ָ��
                 ******************************************************************/
                void* hpserver();
                /******************************************************************
                 * Function��ÿ��������
                 ******************************************************************/
                network::qps* qps();
                inline const ushort port() { return m_port; }

                inline network::http::agent* agent(){return m_agent;}
            public:
                bool m_init_ssl;
                newobj::object_pool<temp_recv>* m_extra_queue;
                newobj::object_pool<newobj::buffer>* m_extra_data_queue;
            private:
                friend class http_server_lst;
			private:
                // HP HttpServer ָ��
                void* m_server;
                // HP Listener ָ��
                network::http::http_server_lst* m_listener;
                // https
                bool m_https;
                // �˿�
                ushort m_port;
                // QPS
                network::qps m_qps;
                // ����ָ��
                network::http::agent *m_agent;
			};
		}
		
		 
	}
}




#endif
