#include "http_router.h"
#if USE_NET_HTTP_WEBSITE
#include "http_reqpack.h"
#include "http_request.h"
#include "http_response.h"
#include "http_server.h"
#include "http_center.h"
#include "http_controller.h"
#include "http_interceptor.h"
#include "http_website.h"
#include "http_agent.h"
#include "hpsocket/hpsocket.h"
#include "util/system.h"
newobj::network::http::router::router()
{
    
    m_threadpool = nullptr;
    m_interceptor = new network::http::interceptor;
    m_threadparam_queue = new newobj::object_pool<thread_param_info>("router",10000,60);
}

newobj::network::http::router::~router()
{
    close();
    delete m_interceptor;
    delete m_threadparam_queue;
}

bool network::http::router::start(const newobj::json &config)
{
    /*启动线程池*/
    {
        uint32 thread_size = config["threadpool"]["size"].to<uint32>();
        uint32 queue_max = config["threadpool"]["queuemax"].to<uint32>();
        if (thread_size > 1)
        {
            m_threadpool = HP_Create_ThreadPool();
            if (this->m_threadpool->Start(
                thread_size,
                queue_max,
                queue_max == 0 ? TRP_WAIT_FOR : TRP_CALL_FAIL, 0) == false)
            {
                //销毁Socket任务线程池
                HP_Destroy_ThreadPool(this->m_threadpool);
                this->m_threadpool = nullptr;
                m_lastErrorDesc =  "threadpool init failed";
                return false;
            }
        }
    }
    ::ithread::start();
    return true;

}

void network::http::router::close()
{
    ::ithread::stop();
    ::ithread::wait();
    if (this->m_threadpool != nullptr)
    {
        this->m_threadpool->Stop(-1);
        delete this->m_threadpool;
        this->m_threadpool = nullptr;
    }
    
}
network::http::interceptor* network::http::router::interceptor(){
    m_interceptor->center(center());
    return m_interceptor;
}

void network::http::router::subscribe(const nstring &path, network::http::method method, std::function<void(network::http::request*,network::http::response*)> callback)
{
    newobj::log->info("[subscribe][func] express:"+path+" method:"+method_to_string(method),"router");
    network::http::subscribe_info *svie = new network::http::subscribe_info;
    svie->controller = false;
    svie->method = method;
    svie->express = std::regex(path.c_str());
    svie->callback = callback;
    m_subscribe.append(svie);
}
void network::http::router::subscribe(
    std::function<void* ()> create_controller_callback,
    network::http::HTTP_CTR_FUNCTION function,
    nstring path,
    network::http::method method
)
{
    newobj::log->info("[subscribe][ctl] express:"+path+" method:"+method_to_string(method),"router");
    network::http::subscribe_info *svie = new network::http::subscribe_info;
    svie->controller = true;
    svie->method = method;
    svie->express = std::regex(path.c_str());
    svie->create_controller_callback = create_controller_callback;
    svie->controller_function = function;
    m_subscribe.append(svie);
};
void network::http::router::other(std::function<void(network::http::request*,network::http::response*)> callback)
{
    this->m_callback_other = callback;
}
void newobj::network::http::router::on_recved(std::function<void(newobj::buffer*)> callback)
{
    m_callback_recved = callback;
}
void newobj::network::http::router::on_sendbefore(std::function<void(newobj::buffer*)> callback)
{
    m_callback_sendbefore = callback;
}
VOID __HP_CALL TaskProc_function(PVOID pvArg)
{
    network::http::router::thread_param_info* tp_info = (network::http::router::thread_param_info*)pvArg;
    tp_info->router->__thread_callback(tp_info->reqpack);

    reqpack::destory(tp_info->reqpack);
    tp_info->router->m_threadparam_queue->destory(tp_info);

//delete tp_info->reqpack;
    //delete tp_info;

}
void newobj::network::http::router::__thread_callback(reqpack* rp)
{

    /*======== ======代理请求==============*/
    if (is_proxy(rp))
        return;
    /*===============正常请求=================*/
    //*拦截器过滤*/
    {
        if (m_interceptor->trigger(rp->filepath(), rp) == false)
        {
            // 已拒绝继续执行
            return;
        }
    }
    bool execed = false;
    for(size_t i=0;i<m_subscribe.m_count;i++){
        auto sub = m_subscribe.get(i);
        if (std::regex_match(rp->filepath().c_str(),sub->express) && (sub->method == rp->method() || sub->method == network::http::ALL))
        {
            execed = true;
            try
            {
                /*开始回调*/
                if (sub->controller)
                {
                    /*controller 类型回调*/
                    std::unique_ptr<network::http::controller> controller((network::http::controller*)sub->create_controller_callback());
                    controller->m_reqpack = rp;
                    controller->center(center());
                    //调用处理函数
                    (controller.get()->*sub->controller_function)();
                }
                else
                {
                    /*普通回调*/
                    sub->callback(rp->request(), rp->response());
                }
                newobj::log->info("["+rp->exec_msec()+" ms] controller url:"+rp->filepath()+" ip:"+rp->request()->remote_ipaddress(true),"router"); 
            }
            catch (const std::exception& e)
            {
                // 通用异常返回
                newobj::log->error("business processing exception:" + nstring(e.what()) + ", url:" + rp->filepath()+" ip:"+rp->request()->remote_ipaddress(true),"router");
                rp->response()->send((nstring)e.what(), 500, "Internal Server Error");
            }
            break;

        }
        
    }
    if(execed == false){
        /*其它回调*/
        if (m_callback_other != nullptr) {
            m_callback_other(rp->request(), rp->response());
        }else{
            rp->response()->send((nstring)"The request will not be processed",410,"Gone");
        }
        newobj::log->warn("["+rp->exec_msec()+" ms] other url:"+rp->filepath()+" ip:"+rp->request()->remote_ipaddress(true),"router"); 
    }
}
size_t newobj::network::http::router::queue_size()
{
    return m_handle_queue.size();
}
void newobj::network::http::router::push(reqpack *rp)
{
    //rp->response()->send((nstring)"FFF");
    //delete rp;
    //return;
    m_handle_queue.push(rp);
    /*日志*/
    {
       // nstring method;
       // if (rp->method() == network::http::GET)
       //     method = "[GET] ";
       // else if (rp->method() == network::http::POST)
       //     method = "[POST] ";
       // center()->log()->info(method + " Filepath：" + rp->filepath());
    }

  
    
}

bool newobj::network::http::router::is_proxy(reqpack* rp)
{
    auto accord_proxy = [](network::http::proxy* proxy,const nstring& filepath)->bool {
        size_t src_length = proxy->src.length();
        if (filepath.left(src_length) == proxy->src)
            return true;
        return false;
    };
    /*获取请求首行，判断代理*/
    auto proxys = rp->website()->proxy();
    for (size_t i = 0; i < proxys->size(); i++)
    {
        network::http::proxy* proxy = proxys->at(i);
        bool accord = accord_proxy(proxy, rp->filepath());
        if (accord == false)
            continue;
        // 临时附加数据
        temp_recv* tr = nullptr;
        {
            PVOID extra = 0;
            if (((IHttpServer*)rp->server()->hpserver())->GetConnectionExtra(rp->connid(), &extra) == false)
                return true;
            tr = (temp_recv*)extra;
            tr->agent = rp->website()->agent();
        }
        timestamp begin_msec = time::now_msec();
        rp->website()->agent()->request(proxy->remote_ipaddress,
            proxy->remote_port,
            3000,
            tr->agent_connid,
            rp->connid(),
            rp->server(),
            rp,
            proxy
            );
        //if (
        //    ((network::http::agent*)tr->agent)->is_connected(tr->agent_connid) == false || 
        //    nstring(proxy->remote_ipaddress+":" + nstring::from(proxy->remote_port)) != tr->ipaddress_port
        //    )
        //{
        //    if(tr->agent_connid != 0)
        //        rp->website()->agent()->disconnect(tr->agent_connid);
        //    tr->ipaddress_port = nstring(proxy->remote_ipaddress + nstring::from(proxy->remote_port));
        //    // 连接
        //    uint64 agent_connid = 0;
        //    if (rp->website()->agent()->connect(
        //        proxy->remote_ipaddress,
        //        proxy->remote_port,
        //        3000,
        //        tr->agent_connid,
        //        rp->connid(),
        //        rp->server()
        //    ) == false)
        //    {
        //        // delete rp;
        //         //abort();
        //        std::cout << "proxy connect failed," << proxy->remote_ipaddress.c_str() << ":" << proxy->remote_port << std::endl;
        //        return true;
        //    }
        //}
        //else
        //{
        //    std::cout << "IsConnect:" << tr->agent_connid << std::endl;
        //}
        //auto a = time::now_msec() - begin_msec;
        //// 发送
        //rp->website()->agent()->send(proxy, tr->agent_connid,rp);
        //std::cout << a<<":"<< time::now_msec() - begin_msec << std::endl;
        return true;
    }
    return false;
}
bool newobj::network::http::router::run()
{
    reqpack* rp = nullptr;
    while (m_handle_queue.pop(rp))
    {
        if (m_threadpool == nullptr)
        {
#if 0
            ((IHttpServer*)rp->server()->hpserver())->SendResponse(rp->connid(), 200, "OK", nullptr, 0, (const BYTE*)"AAA", 3);
            return true;
#endif
            // 单线程
            this->__thread_callback(rp);
            reqpack::destory(rp);
        }
        else
        {
            //thread_param_info* tp_info = new thread_param_info();//m_threadparam_queue->create();
            thread_param_info* tp_info = m_threadparam_queue->create();
            tp_info->reqpack = rp;
            tp_info->router = this;
            if (m_threadpool->Submit(TaskProc_function, (PVOID)tp_info) == FALSE)
            {
                /*线程池繁忙*/
                nstring ipaddress;
                ushort port = 0;
                tp_info->reqpack->server()->remote(rp->connid(), ipaddress, port);
                newobj::log->error("Thread posting failed,Remote: "+ ipaddress+",Filepath: " + rp->filepath(),"router");
                // 发送繁忙回复
                ((IHttpServer*)tp_info->reqpack->server()->hpserver())->SendResponse((CONNID)rp->connid(), 503, "Service Unavailable");
                // 销毁资源
                // 
                reqpack::destory(tp_info->reqpack);
                m_threadparam_queue->destory(tp_info);
                //delete tp_info;
            }
        }
    
    }
    newobj::system::sleep_msec(1);
    return true;
}
#endif
