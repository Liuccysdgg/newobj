#pragma once
#include "network/define.h"
#ifdef USE_NET_TCP_FORWARD
#include <string>
#include "util/thread.h"
#include "tcp_server.h"
#include "util/lock.h"
#include <functional>
#include "util/map.hpp"
#include "tcp_agent.h"
#include "util/lock.h"
namespace newobj
{
    namespace network
    {
        namespace tcp
        {
            namespace forward
            {

                struct forward_data
                {
                    forward_data()
                    {
                        conn_count = 0;
                        conn_online_count = 0;
                        download_byte_size = 0;
                        upload_byte_size = 0;
                    }
                    // 总接收连接次数
                    uint64 conn_count;
                    // 当前在线连接数
                    uint64 conn_online_count;
                    // 总下载流量
                    uint64 download_byte_size;
                    // 总上传流量
                    uint64 upload_byte_size;
                    // 锁
                    newobj::mutex mutex;
                };
                class server:public tcp::server
                {
                public:
                    server();
                    ~server();
                    void on_accept(std::function<bool(uint64 connid, nstring& remote_addr, uint32& remote_port)> callback);
                    void on_server_recv(std::function<void(uint64 connid,newobj::buffer& data)> callback);

                    virtual bool start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode = false, ushort sport_policy = 10000) override;
                public:
                    std::function<bool(uint64 connid, nstring& remote_addr, uint32& remote_port)> m_callback_onaccept;
                    std::function<void(uint64 connid, newobj::buffer& data)> m_callback_server_recv;
                    network::tcp::agent m_agent;
                    forward_data m_runtime_data;
                };
            }
        }
    }
}
#endif