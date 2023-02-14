#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "util/json.h"
#include "util/lock.h"
#include "util/map.hpp"
#include "public/define.h"
#include "public/base.h"
#include "public/buffer.h"

#include <functional>


namespace newobj {
    namespace network {
        namespace http {
            /*������*/
            struct domain_info
            {
                domain_info(){
                    https = false;
                }
                // �Ƿ�HTTPS
                bool https;
                // ����
                nstring domain;
            };

            class server;
            class router;
            class session_mgr;
            class website;
              
            /***************************************************************
             * Class����������
             ***************************************************************/
            class MNETWORK_API center:public newobj::base
            {
            public:
               
                center();
                ~center();
                /******************************************************************
                 * function������
                 * param
                 *      config                  ��           ������
                 * return��
                 *      ʧ�ܿ�ͨ�� last_error() ���ش�����Ϣ��
                 ******************************************************************/
                bool create(const newobj::json& config);
                
                bool start();

                /******************************************************************
                 * function���ر�
                 ******************************************************************/
                void close();

                /******************************************************************
                 * Function��������
                 * Param
                 *      name                      ��           ����
                 * Return
                 *      ��������
                 ******************************************************************/
                nstring read_config(const nstring& name);
                inline const newobj::json& config() { return m_json_config; }
                network::http::server* server(ushort port);

                network::http::website* website(const nstring& host);
            private:
                /*���м����˿�*/
                std::vector<ushort> listen_ports();
                /*�˿��Ƿ���ҪSSL*/
                bool port_have_ssl(ushort port);
            private:
                // HTTP ����
                std::vector<network::http::server*> m_server;
                // վ��
                std::vector<network::http::website*> m_website;
                // ���û���
                newobj::map<nstring,nstring> m_config;
                // JSON����
                newobj::json m_json_config;
            public:
                ptr m_temp[10];

            };
        }
    }
}

#endif
