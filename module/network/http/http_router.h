#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <functional>
#include <map>
#include "util/map.hpp"
#include "public/base.h"
#include "util/json.h"
#include "http_interface.h"
#include "util/queue.hpp"
#include "util/object_pool.hpp"
#include "util/vector.hpp"
#include <regex>
class IHPThreadPool;
namespace newobj {
    namespace network {
        namespace http {
            class reqpack;
            class http_server_lst;
            class request;
            class response;
            class interceptor;
            /*************************************
             * struct：订阅信息
             *************************************/
            struct subscribe_info{
                subscribe_info()
                {
                    method = network::http::ALL;
                    controller = false;
                    controller_function = nullptr;
                }
                // 请求路径
                std::regex express;
                // 请求类型
                network::http::method method;
                // 回调函数
                std::function<void(network::http::request*,network::http::response*)> callback;
                // 控制器。为控制器则启动下面两个属性
                bool controller;
                // 创建控制器类指针
                std::function<void* ()> create_controller_callback;
                // 控制器函数
                HTTP_CTR_FUNCTION controller_function;
            };
            /*************************************************************************
             * class：路由中专服务
             *************************************************************************/
            class MNETWORK_API router:public newobj::base,public network::http::interface_,private newobj::ithread
            {
            public:
                /*线程参数信息*/
                struct thread_param_info
                {
                    void clear(){}
                    network::http::router* router;
                    network::http::reqpack* reqpack;
                };
                
            public:
                router();
                ~router();
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
                /******************************************************************
                 * function：拦截器
                 ******************************************************************/
                network::http::interceptor* interceptor();
                /******************************************************************
                 * function：订阅
                 * desc：浏览器请求会首先触发订阅，订阅未找到符合项则触发 other 传入函数。
                 * param
                 *      path                    ：           路径
                 *      method                  ：           请求类型
                 *      callback                ：           触发回调
                 * return：
                 *      同一地址不允许订阅两次
                 ******************************************************************/
                void subscribe(
                        const nstring& path,
                        network::http::method method,
                        std::function<void(network::http::request*,network::http::response*)> callback
                );
#define SUBSCRIBE(ROUTER,CONTROLLER,FUNCTION,PATH,METHOD) ROUTER->subscribe([]()->void*{return new CONTROLLER;},(network::http::HTTP_CTR_FUNCTION)&CONTROLLER::FUNCTION,PATH,METHOD)

                void subscribe(
                    std::function<void* ()> create_controller_callback,
                    network::http::HTTP_CTR_FUNCTION function,
                    nstring path,
                    network::http::method method
                );

                /******************************************************************
                 * function：其它
                 * desc：未订阅请求触发该回调
                 * param
                 *      callback                ：           触发回调
                 ******************************************************************/
                void other(std::function<void(network::http::request*,network::http::response*)> callback);
                /******************************************************************
                 * function：数据接收后回调
                 ******************************************************************/
                void on_recved(std::function<void(newobj::buffer*)> callback);
                /******************************************************************
                 * function：数据发送前回调
                 * desc：不支持大文件断点传输方式
                 ******************************************************************/
                void on_sendbefore(std::function<void(newobj::buffer*)> callback);
                /******************************************************************
                * function：线程回调[禁止调用]
                * desc：put提交后投递到线程池，线程池开始执行调用该回调。
                * param
                *      param                    ：           线程参数
                ******************************************************************/
                void __thread_callback(reqpack* rq);
                friend class http_server_lst;
                friend class response;
                /******************************************************************
                 * function：队列数
                 ******************************************************************/
                size_t queue_size();

                newobj::object_pool<thread_param_info>* m_threadparam_queue;
            private:
                // 添加任务
                void push(reqpack* rp);
                // 是否为代理任务
                bool is_proxy(reqpack* rp);
            private:
                // 订阅列表
                newobj::nolock_array<network::http::subscribe_info*> m_subscribe;
                // 线程池
                IHPThreadPool* m_threadpool;
                // [回调] 未订阅请求
                std::function<void(network::http::request*, network::http::response*)> m_callback_other;
                // [回调] 接收后
                std::function<void(newobj::buffer*)> m_callback_recved;
                // [回调] 发送前
                std::function<void(newobj::buffer*)> m_callback_sendbefore;
                // 拦截器
                network::http::interceptor* m_interceptor;
            private:
                newobj::queue<network::http::reqpack*> m_handle_queue;
                
                // 通过 ithread 继承
                virtual bool run() override;
};
        }
    }
}
#endif
