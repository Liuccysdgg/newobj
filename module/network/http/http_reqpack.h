#pragma once


#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "public/define.h"
#include "public/buffer.h"
#include "hpsocket/hpsocket.h"
#include "http_server.h"
#include "util/point_pool.hpp"
#include "util/time.h"
namespace newobj {
    namespace network {
        namespace http {
            class server;
            class response;
            class request;
            class website;
            /*************************************************************************
             * class：请求包
             *************************************************************************/
            class MNETWORK_API reqpack
            {
            public:
                static reqpack* create();
                static void destory(reqpack* p);
                
            public:
                
                reqpack();
                ~reqpack();

                void init(const nstring& url, newobj::buffer* data, uint64 connid, network::http::server* server);
                void clear(){}
                network::http::request* request();
                network::http::response* response();

                inline const nstring& host()
                {
                    /*获取基本请求信息*/
                    if (m_host.empty())
                        m_host = ((IHttpServer*)m_server->hpserver())->GetHost((CONNID)m_connid);
                    return m_host;
                }
                inline network::http::method method()
                {
                    if (m_method == network::http::ALL)
                    {
                        nstring method = ((IHttpServer*)m_server->hpserver())->GetMethod((CONNID)m_connid);
                        if (method == "GET")
                            m_method = network::http::GET;
                        else if (method == "POST")
                            m_method = network::http::POST;
                        else
                            m_method = network::http::OTHER;
                    }
                    return m_method;
                }
                inline const nstring& filepath()
                {
                    if (m_filepath.empty())
                    {
                        
                        m_filepath = ((IHttpServer*)m_server->hpserver())->GetUrlField((CONNID)m_connid, EnHttpUrlField::HUF_PATH);
                        // 防止访问上级目录
                        m_filepath = m_filepath.remove("..");
                    }
                    return m_filepath;
                }
                void filepath(const nstring& path);
                inline network::http::server* server()
                {
                    return m_server;
                }
                inline newobj::buffer* data()
                {
                    return m_data;
                }
                inline const nstring& url()
                {
                    return m_url;
                }
                inline const nstring& referrer()
                {
                    return m_referrer;
                }
                inline const uint64& connid()
                {
                    return m_connid;
                }
                inline network::http::website* website() {
                    return m_website;
                }
                inline void website(network::http::website* website) {
                    m_website = website;
                }
                inline timestamp begin_msec(){
                    return m_begin_msec;
                }
                inline nstring exec_msec(){
                    return nstring::from(time::now_msec()-m_begin_msec);
                }
            private:
                // 请求主机
                nstring m_host;
                // 请求方式
                network::http::method m_method;
                // 请求路径
                nstring m_filepath;
                // HPSERVER
                network::http::server *m_server;
                // 转发来路
                nstring m_referrer;
                // 请求ID
                uint64 m_connid;
                // 站点
                network::http::website* m_website;
                // 接收数据
                newobj::buffer* m_data;
                // URL
                nstring m_url;
                // 请求发起时间
                timestamp m_begin_msec;
            private:
                network::http::request* m_request;
                network::http::response* m_response;
            };
        }
    }
}
#endif
