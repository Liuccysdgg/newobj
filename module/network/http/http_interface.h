#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <functional>
#include <map>
#include "public/define.h"

namespace newobj {
    namespace network {
        namespace http {
            /*************************************************************************
             * class��ͨ�ýӿ�
             *************************************************************************/
            class website;
            class center;
            class interface_
            {
            public:
                inline void website(network::http::website* website) { m_website = website; }
                inline network::http::website* website() { return m_website; }
                inline void center(network::http::center* center) { m_center = center; }
                inline network::http::center* center() { return m_center; }
            private:
                network::http::website* m_website;
                network::http::center* m_center;
            };
        }
    }
}
#endif