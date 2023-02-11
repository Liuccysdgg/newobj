#pragma once
#include <vector>
#include <map>
#include "public/define.h"
#include "public/buffer.h"
#include "public/string.h"
#include "network/define.h"
#include <regex>
#define POINT_QUEUE_REQUEST_CLEAR_MAX 1000
#define POINT_QUEUE_REQUEST_CLEAR_SEC 60

#define POINT_QUEUE_RESPONSE_CLEAR_MAX 1000
#define POINT_QUEUE_RESPONSE_CLEAR_SEC 60

#define POINT_QUEUE_ROUTER_CLEAR_MAX 1000
#define POINT_QUEUE_ROUTER_CLEAR_SEC 60

#define POINT_QUEUE_REQPACK_CLEAR_MAX 1000
#define POINT_QUEUE_REQPACK_CLEAR_SEC 60

namespace newobj

{
    namespace network
    {
        namespace http
        {
            // 临时接收
            struct temp_recv
            {
                temp_recv()
                {
                    agent = nullptr;
                    data = nullptr;
                    agent_connid = 0;
                }
                // 接收数据
                newobj::buffer *data;
                // 代理ID
                uint64 agent_connid;
                // 代理服务
                void* agent;
                // URL
                nstring url;
                // 代理连接IP及端口
                nstring ipaddress_port;
            };
            // 代理
            struct proxy
            {
                proxy()
                {
                    remote_port = 0;
                }
                // 拦截地址
                nstring src;
                // 目标地址
                nstring dst;
                // 请求URL
                nstring remote_url;
                // 请求地址
                nstring remote_ipaddress;
                // 请求端口
                ushort remote_port;
                // 主机
                nstring host;
                // 附加协议头
                std::map<nstring, nstring> headers;
            };
            // httpserver代理后连接附加数据
            struct httpserver_proxy_extra
            {
                httpserver_proxy_extra()
                {
                    agent = nullptr;
                    connid = 0;
                    index = 45854;
                }
                void* agent;
                uint64 connid;
                uint64 index;
            };
            struct website_info
            {
                website_info()
                {
                    gzip = false;
                    download_maxbaud = 0;
                }
                // 根目录
                nstring rootdir;
                // GZIP
                bool gzip;
                // 默认编码方式
                nstring default_codec;
                // 带宽限制
                uint32 download_maxbaud;
            };
            /*表单信息*/
            struct form_info
            {
                form_info()
                {
                    start = -1;
                    length = -1;
                }
                nstring disposition;
                nstring content_type;
                nstring name;
                nstring filename;
                int32 start;
                int32 length;
                newobj::buffer data;
            };
            class controller;
            //请求类型
            enum content_type
            {
                CT_FORM = 0,			//表单：	application/x-www-CT_FORM-urlencoded
                CT_JSON = 1,				//CT_JSON:	application/CT_JSON
                CT_TEXT = 2,             //文本：   text/plain
                CT_NIL = 3,               //无
                CT_ALL = 4               //所有
            };
            /*HTTP请求类型*/
            enum method
            {
                GET = 0,
                POST = 1,
                PUT = 2,
                DEL = 3,
                OTHER = 4,
                ALL = 100
            };
            inline nstring method_to_string(enum method m){
                switch(m){
                    case GET:
                        return "GET";
                    case POST:
                        return "POST";
                    case PUT:
                        return "PUT";
                    case DEL:
                        return "DEL";
                    case OTHER:
                        return "OTHER";
                    case ALL:
                        return "ALL";
                    default:
                        return "unknown";
                }
                return "";
            };
            /*控制器内返回结果*/
            enum response_type
            {
                RT_OK,          //成功或已处理
                RT_500,         //服务器内部错误
                RT_406,         //服务器解析客户端数据失败或格式不正确拒绝解析
                RT_401          //需要鉴权，无权限
            };
            //控制器指针
            typedef response_type(network::http::controller::* HTTP_CTR_FUNCTION)();
            /*SSL验证类型*/
            enum ssl_verify_type
            {
                // 完全忽略验证证书的结果。当握手必须完成的话，就选中这个选项。其实真正有证书的人很少，尤其是在中国，那么如果 SSL 运用于一些免费的服务，比如 EMAIL 的时候，SERVER 端最好采用这个模式。
                SVT_VERIFY_NONE = 0x00,
                // 希望验证对方的证书。这个是最一般的模式。对 CLIENT 来说，如果设置了这样的模式，验证SERVER的证书出了任何错误，SSL 握手都告吹。对 SERVER 来说,如果设置了这样的模式，CLIENT 倒不一定要把自己的证书交出去。如果 CLIENT 没有交出证书，SERVER 自己决定下一步怎么做。
                SVT_VERIFY_PEER = 0x01,
                // 这是 SERVER 使用的一种模式，在这种模式下， SERVER 会向 CLIENT 要证书。如果 CLIENT 不给，SSL 握手告吹
                SVT_VERIFY_FAIL_IF_NO_PEER_CERT = 0x02,
                // 这是仅能使用在 SSL SESSION RENEGOTIATION 阶段的一种方式。如果不是用这个模式的话,那么在 RENEGOTIATION 的时候，CLIENT 都要把自己的证书送给 SERVER，然后做一番分析。这个过程很消耗 CPU 时间的，而这个模式则不需要 CLIENT 在 RENEGOTIATION 的时候重复送自己的证书了。
                SVT_VERIFY_CLIENT_ONCE = 0x03

            };
        }

    }
}
