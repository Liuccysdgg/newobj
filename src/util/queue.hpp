#pragma once
#include <queue>
#include "public/define.h"
#include "lock.h"
#include <stdio.h>
namespace newobj
{
	template <typename T>
	class queue : private std::queue<T>
	{
	public:
		queue<T>()
		{
		}
		~queue<T>()
		{
		}
		void push(const T& param)
		{
			this->m_mutex.lock();
			::std::queue<T>::push(param);
			this->m_mutex.unlock();
		}
		bool pop(T& param)
		{
			this->m_mutex.lock();
			if (this->empty())
			{
				this->m_mutex.unlock();
				return false;
			}

			param = ::std::queue<T>::front();
			::std::queue<T>::pop();
			this->m_mutex.unlock();
			return true;
		}
		size_t size()
		{
			return ::std::queue<T>::size();
		}
		void clear()
		{
			this->m_mutex.lock();
			while (this->empty() == false)
				::std::queue<T>::pop();
			this->m_mutex.unlock();
		}

	private:
		newobj::mutex m_mutex;
	};
}

