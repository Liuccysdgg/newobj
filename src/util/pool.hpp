#pragma once
#include "public/define.h"
#include "public/base.h"
#include "util/thread.h"
#include "util/system.h"
#include "public/exception.h"
#include <iostream>
namespace newobj {

    class example_core{
    public:
        virtual void recover() = 0;
        virtual void task_out() = 0;
    };
    class NEWOBJ_API poolcore:public newobj::base
    {
    public:
        void recover(void* example)
        {
		std::cout<<"[push]:"<<std::hex<<example<<std::endl;
            if(example == NULL)
                return;
            ((newobj::example_core*)example)->recover();
            ____lock____(m_mutex,true);
            m_pop_size--;
            m_queue.push(example);
        }
    protected:
        newobj::queue<void*> m_queue;
        newobj::mutex m_mutex;
        bool m_closing;
        size_t m_max_size;
        size_t m_pop_size;
    };
    template<typename INFO>
    class NEWOBJ_API example:public example_core
    {
    public:
        example()
        {
            m_pool = nullptr;
        }
        virtual bool start(const INFO& info) = 0;
        virtual void close() = 0;

        inline void pool(poolcore *pool)
        {
            m_pool = pool;
        }
        inline poolcore *pool()
        {
            return m_pool;
        }
    protected:
        poolcore *m_pool;
    };

    template<typename EXAMPLE>
    /*自动连接释放器*/
    class NEWOBJ_API conn_autofree
    {
    public:
        conn_autofree(EXAMPLE* conn){
            m_conn = conn;
        }
        ~conn_autofree(){
            if (m_conn != nullptr)
            {
                m_conn->pool()->recover(m_conn);
            }
        }
        EXAMPLE* operator->() {
            if (m_conn == nullptr)
            {
                trw_str("The connection is nullptr and an exception is thrown");
            }
            return m_conn;
        }
        EXAMPLE* get(){
		if(m_conn == nullptr){
		
			trw_str("conn is nullptr");
		}
            return m_conn;
        }
    private:
        EXAMPLE* m_conn;
    };


    template<typename EXAMPLE,typename INFO>
    class NEWOBJ_API pool :public poolcore
    {
    public:
        pool<EXAMPLE,INFO>()
        {
            m_max_size = 0;
            m_pop_size = 0;
            m_closing = false;
        }
        ~pool<EXAMPLE,INFO>()
        {
            close();
        }
        bool start(const INFO& info,size_t size)
        {
            m_info = info;
            m_max_size = size;
            return true;
        }
        void close()
        {
            ____lock____(m_mutex, true);
            m_closing = true;
            for (size_t i = 0; i < m_queue.size(); i++)
            {
                void* example = NULL;
                if (m_queue.pop(example) == false)
                    break;
                EXAMPLE*f = ((EXAMPLE*)example);
                ((EXAMPLE*)example)->pool(nullptr);
                delete ((EXAMPLE*)example);
            }
        }

        EXAMPLE* get()
        {
            ____lock____(m_mutex, true);
            if (m_closing)
            {
                std::cout << "pool is shutting down" << std::endl;
                return NULL;
            }
            void* example = nullptr;
            if (m_queue.pop(example) == false)
            {
                if (m_pop_size < m_max_size)
                {
                    bool init_success = false;
                    example = new EXAMPLE;
                    for (uint32 i = 0; i < 3; i++)
                    {
                        if (((EXAMPLE*)example)->start(m_info))
                        {
                            init_success = true;
                            break;
                        }
                        else
                        {
                            std::cout << "start failed." << ((EXAMPLE*)example)->last_error().c_str() << std::endl;
                            std::cout << "restart "<<nstring::from(i+1).c_str()<< "." << std::endl;
                        }
                    }
                    if (init_success)
                    {
                        m_pop_size++;
                        ((EXAMPLE*)example)->pool(this);
                        ((EXAMPLE*)example)->task_out();
                        return ((EXAMPLE*)example);
                    }
                    else
                    {
                        delete ((EXAMPLE*)example);
                        return NULL;
                    }
                }
                else
                {
                    std::cout << "maximum capacity exceeded" << std::endl;
                    return NULL;
                }
            }
            else
            {
                m_pop_size++;
                ((EXAMPLE*)example)->pool(this);
                ((EXAMPLE*)example)->task_out();
                return ((EXAMPLE*)example);
            }
            return NULL;
        }
        size_t size()
        {
            return m_max_size-m_pop_size;
        }
    public:
        INFO m_info;
    };
}
