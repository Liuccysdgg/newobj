#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "public/define.h"
#include "public/string.h"
#include "public/base.h"
#include "util/lock.h"
#include <functional>
#include "http_interface.h"
#include <map>
#ifdef LIB_HPSOCKET_SSL
namespace newobj {
    namespace network {
        namespace http {
            class server;
            /******************************************************
             * class��������
             ******************************************************/
            class MNETWORK_API ssl :public newobj::base, public network::http::interface_
            {
            public:
                /**********************************************************************************************
                 * function�����캯��
                 * param
                 *          server                                      ��                                   HTTP����
                 *          verify_type                               ��                                  ��֤��ʽ
                 *          pem_cert                                 ��                                   ֤������
                 *          pem_key                                  ��                                   ֤����Կ
                 *          password                                 ��                                   ֤�����룬û������
                 **********************************************************************************************/
                ssl(network::http::server* server,ssl_verify_type verify_type,const nstring& pem_cert,const nstring&pem_key,const nstring& password = "");
                ~ssl();
                /**********************************************************************************************
                 * function��ע��֤��
                 **********************************************************************************************/
                bool regist();
                /**********************************************************************************************
                 * function����
                 **********************************************************************************************/
                bool bind(const nstring& host);
            private:
                ssl_verify_type m_verify_type;
                nstring m_pem_cert;
                nstring m_pem_key;
                nstring m_pem_password;
                network::http::server* m_server;
                int32 m_index;
            };
        }
    }

}

#endif
#endif