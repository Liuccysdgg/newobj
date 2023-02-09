#pragma once
#include "public/define.h"
#include <functional>
#include "json.h"
namespace newobj
{ 
    typedef void(*CALLBACK_THREA_CREATE_FUNCTION)(ptr param);
    /****************************************************
        * Class：线程工具类
        * DateTime：2019-10-29
        ****************************************************/
    class  NEWOBJ_API ithread
    {
    public:
        ithread();
        ~ithread();
        /***************************************************************************
         * Function：开启线程
         * Return:
         *			==	true		:		成功
         *			==	false		:		失败
         ****************************************************************************/
        bool start();
        bool state();
        void wait();
        /***************************************************************************
         * Function：停止线程
         * Return:
         *			==	true		:		成功
         *			==	false		:		失败
         ****************************************************************************/
        bool stop();
    private:
        /***************************************************************************
         * Function：纯虚函数 需要子类实现
         * Description：本类会线程内调用该类,间歇时间由 Run() 内自行实现
         * Return:
         *			==	true		:		继续执行下一循环
         *			==	false		:		终止循环，退出线程
         ****************************************************************************/
        virtual bool run() = 0;
    public:
        static void* __thread_handle(void* param);
    public:
        unsigned int m_state;
        bool m_thread;
    public:

        
        
        
    };
    namespace thread
    {
        /*
        * 创建线程并设置等待，不建议在严谨的生产环境中使用。
        */
        bool NEWOBJ_API create(std::function<void(void*)> func, void* param,std::function<bool/*true=等待线程结束 false=不等待*/()> wait);
        bool NEWOBJ_API create(std::function<void(void*)> func, void* param, bool wait = false);
    }
    /****************************************************
        * Class：线程工具类
        * DateTime：2013-01-11
        ****************************************************/
    class  NEWOBJ_API thread_pool
    {
    public:
        thread_pool();
        ~thread_pool();
        bool start(uint32 thread_count,uint32 max_queue_size);
        bool submit(void* param);
    private:
        /***************************************************************************
         * Function：纯虚函数 需要子类实现
         ****************************************************************************/
        virtual void run_pool(void* param) = 0;
    public:
        void __thread_handle(void* param);
        void* m_hp_thread;
    };
}