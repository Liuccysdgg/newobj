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
    /*�����̳߳�*/
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
                //����Socket�����̳߳�
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

    /*======== ======��������==============*/
    if (is_proxy(rp))
        return;
    /*===============��������=================*/
    //*����������*/
    {
        if (m_interceptor->trigger(rp->filepath(), rp) == false)
        {
            // �Ѿܾ�����ִ��
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
                /*��ʼ�ص�*/
                if (sub->controller)
                {
                    /*controller ���ͻص�*/
                    std::unique_ptr<network::http::controller> controller((network::http::controller*)sub->create_controller_callback());
                    controller->m_reqpack = rp;
                    controller->center(center());
                    //���ô�����
                    (controller.get()->*sub->controller_function)();
                }
                else
                {
                    /*��ͨ�ص�*/
                    sub->callback(rp->request(), rp->response());
                }
                newobj::log->info("["+rp->exec_msec()+" ms] controller url:"+rp->filepath()+" ip:"+rp->request()->remote_ipaddress(true),"router"); 
            }
            catch (const std::exception& e)
            {
                // ͨ���쳣����
                newobj::log->error("business processing exception:" + nstring(e.what()) + ", url:" + rp->filepath()+" ip:"+rp->request()->remote_ipaddress(true),"router");
                rp->response()->send((nstring)e.what(), 500, "Internal Server Error");
            }
            break;

        }
        
    }
    if(execed == false){
        /*�����ص�*/
        if (m_callback_other != nullptr) {
            m_callback_other(rp->request(), rp->response());
        }else{
            nstring filepath = rp->request()->filepath();
            if(filepath == "/")
                filepath = "/index.html";
            if(rp->response()->send_file(filepath) == false){
                rp->response()->send((nstring)"404 Not found",404,"Not Found");        
            }
        }
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
    /*��־*/
    {
       // nstring method;
       // if (rp->method() == network::http::GET)
       //     method = "[GET] ";
       // else if (rp->method() == network::http::POST)
       //     method = "[POST] ";
       // center()->log()->info(method + " Filepath��" + rp->filepath());
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
    /*��ȡ�������У��жϴ���*/
    auto proxys = rp->website()->proxy();
    for (size_t i = 0; i < proxys->size(); i++)
    {
        network::http::proxy* proxy = proxys->at(i);
        bool accord = accord_proxy(proxy, rp->filepath());
        if (accord == false)
            continue;
        timestamp begin_msec = time::now_msec();
        rp->server()->agent()->request(3000,rp,proxy);
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
            // ���߳�
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
                /*�̳߳ط�æ*/
                nstring ipaddress;
                ushort port = 0;
                tp_info->reqpack->server()->remote(rp->connid(), ipaddress, port);
                newobj::log->error("Thread posting failed,Remote: "+ ipaddress+",Filepath: " + rp->filepath(),"router");
                // ���ͷ�æ�ظ�
                ((IHttpServer*)tp_info->reqpack->server()->hpserver())->SendResponse((CONNID)rp->connid(), 503, "Service Unavailable");
                // ������Դ
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
