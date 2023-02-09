#pragma once
#include "public/define.h"
#include "util/queue.hpp"
#include "util/time.h"
#include <iostream>
namespace newobj
{
	template<typename T>
	class point_pool {
	public:
		point_pool(const nstring& name,uint32 clear_size,uint32 clear_sec)
		{
			m_name = name;
			if (clear_size == 0 || clear_size > 999999999)
				clear_size = 1;
			if (clear_sec == 0)
				clear_sec = 100;
			m_clear_size = clear_size;
			m_clear_sec = clear_sec;
			m_prev_sec = time::now_sec();
		}
		~point_pool()
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
					std::cout << "point_queue :" << m_name.c_str() << " ,clear:" << clear_size << std::endl;
				}
			}
			T* p = nullptr;
			if (m_queue.pop(p))
				return p;
			return new T();
		}
		void destory(T* p) 
		{
            p->clear();
			m_queue.push(p);
		}
	private:
		newobj::queue<T*> m_queue;
		timestamp m_prev_sec;
		uint32 m_clear_size;
		uint32 m_clear_sec;
		nstring m_name;
	};
}
