#pragma once
#include "public/define.h"
#include "db_redis.h"
#include "db_mysql.h"
#ifndef MSVC_2010
/*自动释放指针*/
template <typename T>
class conn_ptr
{
public:
    conn_ptr(T* p)
    {
        m_p = p;
    }
    conn_ptr()
    {
        m_p = NULL;
    }
    conn_ptr(ptr connPool)
    {
        m_connPool = connPool;
    };
    ~conn_ptr()
    {
        if (m_p != nullptr)
            delete m_p;
    }
    T* operator->()
    {

        if (m_p == nullptr)
        {
            ____lock____(this->m_mutex, true);
            if (m_p == nullptr && m_connPool != 0)
            {
                m_p = new T(m_connPool);
            }
        }
        return m_p;
    }
    T*& operator=(T* p)
    {
        ____lock____(this->m_mutex, true);
        if (p == nullptr)
        {
            if (m_p == nullptr)
            {
                if (m_connPool != 0)
                {
                    m_p = new T(m_connPool);
                }
            }
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
    T* get()
    {
        return operator=(nullptr);
    }
    long long get_ptr()
    {
        return (long long)(void*)m_p;
    }
    void setPoolPtr(long long pool)
    {
        ____lock____(this->m_mutex, true);
        m_connPool = pool;
    }
private:
    T* m_p = nullptr;
    newobj::mutex m_mutex;
public:
    long long m_connPool = 0;
};
#endif