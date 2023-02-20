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
#include <regex>
#include "util/vector.hpp"
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
                 * function：启动
                 * param
                 *      config                  ：           配置项
                 * return：
                 *      失败可通过 last_error() 返回错误信息。
                 ******************************************************************/
                bool start(const newobj::json& config);
                /******************************************************************
                 * function：关闭
                 ******************************************************************/
                void close();

                network::http::router* router();
                network::http::session_mgr* session();
                bool host(const nstring& host);
                newobj::nolock_array<network::http::proxy*>* proxy();
                const network::http::website_info* info();
/*                inline network::http::agent* agent() { return m_agent; }*/
            private:
                // SESSION缓存
                network::http::session_mgr* m_session;
                // router路由 服务
                network::http::router* m_router;
                // 配置信息
                struct website_info m_info;
                // HOST
                std::vector<network::http::host*> m_hosts;
                // HTTPS
                bool m_https;
                // 反向代理
                newobj::nolock_array<network::http::proxy*> m_proxy;
            };
        }
    }
}

#endif
