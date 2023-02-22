#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "public/define.h"
#include "basetype/string.h"
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
             * class：域名器
             ******************************************************/
            class MNETWORK_API ssl :public newobj::base, public network::http::interface_
            {
            public:
                /**********************************************************************************************
                 * function：构造函数
                 * param
                 *          server                                      ：                                   HTTP服务
                 *          verify_type                               ：                                  验证方式
                 *          pem_cert                                 ：                                   证书内容
                 *          pem_key                                  ：                                   证书密钥
                 *          password                                 ：                                   证书密码，没有留空
                 **********************************************************************************************/
                ssl(network::http::server* server,ssl_verify_type verify_type,const nstring& pem_cert,const nstring&pem_key,const nstring& password = "");
                ~ssl();
                /**********************************************************************************************
                 * function：注册证书
                 **********************************************************************************************/
                bool regist();
                /**********************************************************************************************
                 * function：绑定
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