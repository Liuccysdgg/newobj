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
#include "log/log_no.h"


namespace newobj {
    namespace network {
        namespace http {
            /*绑定域名*/
            struct domain_info
            {
                domain_info(){
                    https = false;
                }
                // 是否HTTPS
                bool https;
                // 域名
                nstring domain;
            };

            class server;
            class router;
            class session_mgr;
            class website;
              
            /***************************************************************
             * Class：控制中心
             ***************************************************************/
            class MNETWORK_API center:public newobj::base
            {
            public:
               
                center();
                ~center();
                /******************************************************************
                 * function：创建
                 * param
                 *      config                  ：           配置项
                 * return：
                 *      失败可通过 last_error() 返回错误信息。
                 ******************************************************************/
                bool create(const newobj::json& config);
                
                bool start();

                /******************************************************************
                 * function：关闭
                 ******************************************************************/
                void close();

                /******************************************************************
                 * Function：读配置
                 * Param
                 *      name                      ：           名称
                 * Return
                 *      配置内容
                 ******************************************************************/
                nstring read_config(const nstring& name);
                inline const newobj::json& config() { return m_json_config; }
                network::http::server* server(ushort port);

                network::http::website* website(const nstring& host);

                newobj::log::no* log();
            private:
                /*所有监听端口*/
                std::vector<ushort> listen_ports();
                /*端口是否需要SSL*/
                bool port_have_ssl(ushort port);
            private:
                // HTTP 服务
                std::vector<network::http::server*> m_server;
                // 站点
                std::vector<network::http::website*> m_website;
                // 配置缓存
                newobj::map<nstring,nstring> m_config;
                // 日志服务
                newobj::log::no *m_log;
                // JSON配置
                newobj::json m_json_config;
            public:
                ptr m_temp[10];

            };
        }
    }
}

#endif