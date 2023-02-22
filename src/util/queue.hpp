#pragma once
#include <queue>
#include "public/define.h"
#include "lock.h"
#include <atomic>
#include <stdio.h>
namespace newobj
{
    template<typename T>
    class queue
    {
    public:
        struct node
        {
            T value;
            node* next;
        };
    public:
        queue<T>()
        {
            m_first = nullptr;
            m_end = nullptr;
            m_size = 0;
        }
        ~queue<T>()
        {
            clear();
        }
        inline void clear()
        {
            T value;
            while(pop(value));

        }
        inline void push(const T& value)
        {
            ____lock____(m_mutex,true); 
            m_size++;
            if(m_first ==nullptr)
            {
                m_first = new node();
                m_first->next  =  nullptr;
                m_first->value = value;
                m_end = m_first;
                return;
            }
            m_end->next = new node;
            m_end->next->next = nullptr;
            m_end->next->value = value;
            m_end = m_end->next;
        }
        inline bool pop(T& value,bool locked = true)
        {
            if(m_first == nullptr)
                return false;
            ____lock____(m_mutex,locked); 
            if(m_first == nullptr)
                return false;           
            m_size--;
            value = m_first->value;
            node* temp = m_first->next;
            delete m_first;
            m_first = temp;
            return true;
        }
        inline size_t size(){return 0;}
        node* m_first;
        node* m_end;
        newobj::mutex m_mutex;
        size_t m_size;
    };
}

