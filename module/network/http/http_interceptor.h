#pragma once
#include "network/define.h"
#if USE_NET_HTTP_WEBSITE
#include "public/define.h"
#include "public/string.h"
#include "public/base.h"
#include "util/lock.h"
#include "util/map.hpp"
#include <functional>
#include "http_interface.h"
namespace newobj {
    namespace network {
        namespace http {
            class reqpack;
            /******************************************************
             * class£ºÀ¹½ØÆ÷
             ******************************************************/
            class MNETWORK_API interceptor :public newobj::base, public network::http::interface_
            {
            public:
                interceptor();
                ~interceptor();
                bool add(const nstring& path,std::function<bool(network::http::reqpack* rp)> callback);
                bool remove(const nstring& path);
                bool trigger(const nstring& url,network::http::reqpack* rp);
            private:
                newobj::map<nstring, std::function<bool(network::http::reqpack* rp)>> m_values;
            };
        }
    }

}

#endif