#pragma once
#include "network/define.h"
#ifdef USE_NET_TCP_CLIENT
#include <string>
#include "util/thread.h"
#include "network/iclient.h"
#include "util/lock.h"
namespace newobj
{
    namespace network
    {
        namespace tcp
        {
            class ctllistener;



            /*接收数据结构*/
            struct TcpClientRecvSt
            {
                TcpClientRecvSt()
                {
                    pData = NULL;
                    iLength = 0;
                    pClient = NULL;
                    callback = NULL;
                }
                char* pData;
                uint32 iLength;
                ::newobj::network::iclient* pClient;
                std::function<void(::newobj::network::iclient*, const char*, uint32)> callback;
            };

            class MNETWORK_API client :public iclient, private ::newobj::ithread
            {
            public:
                client(::network::TcpType type);
                ~client();
                // 通过 IClient 继承
                virtual bool start(const nstring& address, uint32 port, bool reconnect = false, bool sportMode = false) override;
                virtual bool start(bool reconnect = false, bool sportMode = false);
                virtual bool disconn() override;
                virtual bool close(uint32 wait_msec = 0) override;
                virtual bool connect(const nstring& address = "", uint32 port = 0) override;
                virtual bool connect(const nstring& address, uint32 port, int32 wait_msec);
                virtual bool conned() override;
                virtual void remote(const nstring& address, uint32 port, bool reconn = true) override;
                virtual bool send(const char* buff, uint32 len) override;
                virtual void send(const buffer& data) override;
                // 通过 Thread 继承
                virtual bool run() override;

            private:
                ::newobj::mutex m_mutex;
                ::network::TcpType m_type;
            public:
                void on_recv(std::function<void(::newobj::network::iclient*, const char*, uint32)> cb);
                 
                void on_accept(std::function<void(::newobj::network::iclient*)> cb);
                void on_close(std::function<void(::newobj::network::iclient*)> cb);
                void on_filter(std::function<void(::newobj::network::iclient*, uint32)> cb);
            public:
                std::function<void(::newobj::network::iclient*)> m_pfun_onaccept;
                std::function<void(::newobj::network::iclient*)> m_pfun_onclose;
                std::function<void(::newobj::network::iclient*, const char*, uint32)> m_pfun_onrecv;
                std::function<void(::newobj::network::iclient*, uint32)> m_pfun_onfilter;
            };
        }
    }
}
#endif