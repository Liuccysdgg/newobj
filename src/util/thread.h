#pragma once
#include "public/define.h"
#include <functional>
#include "json.h"
namespace newobj
{ 
    typedef void(*CALLBACK_THREA_CREATE_FUNCTION)(ptr param);
    /****************************************************
        * Class���̹߳�����
        * DateTime��2019-10-29
        ****************************************************/
    class  NEWOBJ_API ithread
    {
    public:
        ithread();
        ~ithread();
        /***************************************************************************
         * Function�������߳�
         * Return:
         *			==	true		:		�ɹ�
         *			==	false		:		ʧ��
         ****************************************************************************/
        bool start();
        bool state();
        void wait();
        /***************************************************************************
         * Function��ֹͣ�߳�
         * Return:
         *			==	true		:		�ɹ�
         *			==	false		:		ʧ��
         ****************************************************************************/
        bool stop();
    private:
        /***************************************************************************
         * Function�����麯�� ��Ҫ����ʵ��
         * Description��������߳��ڵ��ø���,��Ъʱ���� Run() ������ʵ��
         * Return:
         *			==	true		:		����ִ����һѭ��
         *			==	false		:		��ֹѭ�����˳��߳�
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
        * �����̲߳����õȴ������������Ͻ�������������ʹ�á�
        */
        bool NEWOBJ_API create(std::function<void(void*)> func, void* param,std::function<bool/*true=�ȴ��߳̽��� false=���ȴ�*/()> wait);
        bool NEWOBJ_API create(std::function<void(void*)> func, void* param, bool wait = false);
    }
    /****************************************************
        * Class���̹߳�����
        * DateTime��2013-01-11
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
         * Function�����麯�� ��Ҫ����ʵ��
         ****************************************************************************/
        virtual void run_pool(void* param) = 0;
    public:
        void __thread_handle(void* param);
        void* m_hp_thread;
    };
}