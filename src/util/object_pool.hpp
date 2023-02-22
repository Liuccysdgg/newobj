#pragma once
#include "public/define.h"
#include "util/queue.hpp"
#include "util/time.h"
#include <iostream>
namespace newobj
{

	struct iop
	{
	public:
		virtual void clear() = 0;
	};

	template<typename T>
	class object_pool {
	public:
		object_pool(const nstring& name,uint32 clear_size,uint32 clear_sec)
		{
            m_lock = true;
			m_name = name;
			if (clear_sec == 0)
				clear_sec = 100;
			m_clear_size = clear_size;
			m_clear_sec = clear_sec;
			m_prev_sec = time::now_sec();
		}
		~object_pool()
		{
			T* p = nullptr;
			while (m_queue.pop(p))
			{
				delete p;
			}
		}
		T* create() 
		{
            
			if (m_queue.size() > m_clear_size)
			{
				if (time::now_sec() > m_clear_sec + m_prev_sec)
				{
					uint32 clear_size = 0;
					T* p = nullptr;
					while(m_queue.pop(p))
					{
						clear_size++;
						delete p;
						if (m_queue.size() < m_clear_size)
							break;
					}
					m_prev_sec = time::now_sec();
				}
			}
			T* p = nullptr;
			if (m_queue.pop(p))
            {
				return p;
            }
			return new T();
		}
		void destory(T* p) 
		{
            p->clear();
			m_queue.push(p);
		}
	private:
        bool m_lock;
	    newobj::queue<T*> m_queue;
		timestamp m_prev_sec;
		uint32 m_clear_size;
		uint32 m_clear_sec;
		nstring m_name;
	};
}
