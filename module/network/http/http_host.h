#pragma once
#include "network/define.h"
#if USE_NET_HTTP_WEBSITE
#include "public/define.h"
#include "public/string.h"
#include "public/base.h"
#include "util/lock.h"
#include <functional>
#include "http_interface.h"
#include <map>
namespace newobj {
    namespace network {
        namespace http {
            /******************************************************
             * class£ºÓòÃûÆ÷
             ******************************************************/
            class ssl;
            class MNETWORK_API host :public newobj::base, public network::http::interface_
            {
            public:
                host();
                ~host();
                void init(const nstring& host,ushort port, network::http::ssl* ssl = nullptr);
                inline network::http::ssl* ssl(){return m_ssl;}
                inline bool equal(const nstring& host) { return m_host == host; }
                inline const nstring& hostname() { return m_return_host; }
            private:
                nstring m_host;
                nstring m_return_host;
                ushort m_port;
                network::http::ssl* m_ssl;
            };
        }
    }

}

#endif