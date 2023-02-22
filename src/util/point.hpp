#pragma once
#include "public/define.h"
//#ifndef MSVC_2010
namespace newobj
{
    /*自动释放指针*/
    template <typename T>
    class auto_free
    {
    public:
        auto_free(T* p)
        {
            m_p = p;
        }
        auto_free()
        {
            m_p = nullptr;
        };
        ~auto_free()
        {
            if (m_p != nullptr)
                delete m_p;
        }
        T* operator->()
        {
            return m_p;
        }
        T* operator=(T* p)
        {
            if (p == nullptr)
            {
                /*if (m_p == nullptr)
                    m_p = new T;*/
                return m_p;
            }
            if (m_p != nullptr)
                delete m_p;
            m_p = p;
            return m_p;
        }
        bool operator==(T* p)
        {
            return m_p == p;
        }
           
        long long get_ptr()
        {
            return (long long)m_p;
        }
        T* get()
        {
            return m_p;
        }
    private:
        T* m_p;
    };
}
//#endif