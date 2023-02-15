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
             * struct��������Ϣ
             *************************************/
            struct subscribe_info{
                subscribe_info()
                {
                    method = network::http::ALL;
                    controller = false;
                    controller_function = nullptr;
                }
                // ����·��
                std::regex express;
                // ��������
                network::http::method method;
                // �ص�����
                std::function<void(network::http::request*,network::http::response*)> callback;
                // ��������Ϊ������������������������
                bool controller;
                // ������������ָ��
                std::function<void* ()> create_controller_callback;
                // ����������
                HTTP_CTR_FUNCTION controller_function;
            };
            /*************************************************************************
             * class��·����ר����
             *************************************************************************/
            class MNETWORK_API router:public newobj::base,public network::http::interface_,private newobj::ithread
            {
            public:
                /*�̲߳�����Ϣ*/
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
                 * function������
                 * param
                 *      config                  ��           ������
                 * return��
                 *      ʧ�ܿ�ͨ�� last_error() ���ش�����Ϣ��
                 ******************************************************************/
                bool start(const newobj::json& config);
                /******************************************************************
                 * function���ر�
                 ******************************************************************/
                void close();
                /******************************************************************
                 * function��������
                 ******************************************************************/
                network::http::interceptor* interceptor();
                /******************************************************************
                 * function������
                 * desc���������������ȴ������ģ�����δ�ҵ��������򴥷� other ���뺯����
                 * param
                 *      path                    ��           ·��
                 *      method                  ��           ��������
                 *      callback                ��           �����ص�
                 * return��
                 *      ͬһ��ַ������������
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
                 * function������
                 * desc��δ�������󴥷��ûص�
                 * param
                 *      callback                ��           �����ص�
                 ******************************************************************/
                void other(std::function<void(network::http::request*,network::http::response*)> callback);
                /******************************************************************
                 * function�����ݽ��պ�ص�
                 ******************************************************************/
                void on_recved(std::function<void(newobj::buffer*)> callback);
                /******************************************************************
                 * function�����ݷ���ǰ�ص�
                 * desc����֧�ִ��ļ��ϵ㴫�䷽ʽ
                 ******************************************************************/
                void on_sendbefore(std::function<void(newobj::buffer*)> callback);
                /******************************************************************
                * function���̻߳ص�[��ֹ����]
                * desc��put�ύ��Ͷ�ݵ��̳߳أ��̳߳ؿ�ʼִ�е��øûص���
                * param
                *      param                    ��           �̲߳���
                ******************************************************************/
                void __thread_callback(reqpack* rq);
                friend class http_server_lst;
                friend class response;
                /******************************************************************
                 * function��������
                 ******************************************************************/
                size_t queue_size();

                newobj::object_pool<thread_param_info>* m_threadparam_queue;
            private:
                // �������
                void push(reqpack* rp);
                // �Ƿ�Ϊ��������
                bool is_proxy(reqpack* rp);
            private:

                // �����б�
                newobj::nolock_array<network::http::subscribe_info*> m_subscribe;
                // �̳߳�
                IHPThreadPool* m_threadpool;
                // [�ص�] δ��������
                std::function<void(network::http::request*, network::http::response*)> m_callback_other;
                // [�ص�] ���պ�
                std::function<void(newobj::buffer*)> m_callback_recved;
                // [�ص�] ����ǰ
                std::function<void(newobj::buffer*)> m_callback_sendbefore;
                // ������
                network::http::interceptor* m_interceptor;
            private:
                newobj::queue<network::http::reqpack*> m_handle_queue;
                
                // ͨ�� ithread �̳�
                virtual bool run() override;
};
        }
    }
}
#endif
