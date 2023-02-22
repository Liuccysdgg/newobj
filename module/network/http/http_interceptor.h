#pragma once
#include "network/define.h"
#if USE_NET_HTTP_WEBSITE
#include "public/define.h"
#include "basetype/string.h"
#include "public/base.h"
#include "util/lock.h"
#include "util/map.hpp"
#include <functional>
#include <regex>
#include "http_interface.h"
namespace newobj {
    namespace network {
        namespace http {
            class reqpack;

            struct interceptor_info{
                std::regex express;
                nstring express_string;
                std::function<bool(network::http::reqpack* rp)> callback;
            };
            /******************************************************
             * class£ºÀ¹½ØÆ÷
             ******************************************************/
            class MNETWORK_API interceptor :public newobj::base, public network::http::interface_
            {
            public:
                interceptor();
                ~interceptor();
                size_t add(const nstring& regex_express,std::function<bool(network::http::reqpack* rp)> callback);
                void remove(size_t index);
                bool trigger(const nstring& url,network::http::reqpack* rp);
            private:
                newobj::nolock_array<interceptor_info*> m_array;
            };
        }
    }

}

#endif
