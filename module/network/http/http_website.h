#pragma once
#include "http_define.h"
#if USE_NET_HTTP_UTIL
#include "public/define.h"
#include "public/string.h"
#include "public/base.h"
#include "util/lock.h"
#include "util/json.h"
#include <functional>
#include "http_interface.h"
#include <map>
namespace newobj {
    namespace network {
        namespace http {
            class server;
            class router;
            class session_mgr;
            class host;
            class agent;
            class MNETWORK_API website :public newobj::base, public network::http::interface_
            {
            public:
                
            public:
                website();
                ~website();
                /******************************************************************
                 * function������
                 * param
                 *      config                  ��           ������
                 * return��
                 *      ʧ�ܿ�ͨ�� last_error() ���ش�����Ϣ��
                 ******************************************************************/
                bool start(const newobj::json& config);
                /******************************************************************
                 * function���ر�
                 ******************************************************************/
                void close();

                network::http::router* router();
                network::http::session_mgr* session();
                bool host(const nstring& host);
                std::vector<network::http::proxy*>* proxy();
                const network::http::website_info* info();
/*                inline network::http::agent* agent() { return m_agent; }*/
            private:
                // SESSION����
                network::http::session_mgr* m_session;
                // router·�� ����
                network::http::router* m_router;
                // ������Ϣ
                struct website_info m_info;
                // HOST
                std::vector<network::http::host*> m_hosts;
                // HTTPS
                bool m_https;
                // �������
                std::vector<network::http::proxy*> m_proxy;
                // �������
/*                network::http::agent* m_agent;*/
            };
        }
    }
}

#endif
