#pragma once
#include "public/define.h"
#include <iostream>
#include <map>
#include "util/timeout.h"
#include "vector.hpp"
namespace newobj
{
	template <typename KEY, typename VAL>
	class map:private std::map<KEY,VAL>
	{
	public:
		map<KEY, VAL>(bool lock = true) {
			m_lock = lock;
		};
		~map<KEY, VAL>() {};
		const std::map<KEY, VAL> to_stl()
		{
			____lock____(this->m_mutex, m_lock);
			return *this;
		}

		bool add(const KEY& key_, const VAL val, bool lock = true)
		{
			if (lock)
				this->m_mutex.lock();
			typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY,VAL>::find(key_);
			if (iter != ::std::map<KEY, VAL>::end()) {
				if (lock)
					this->m_mutex.unlock();
				return false;
			}

			::std::map<KEY, VAL>::insert(std::pair<KEY, VAL>(key_, val));
			if (lock)
				this->m_mutex.unlock();
			return true;
		}
		bool exist(const KEY& key_, bool lock = true)
		{



			if (lock)
				this->m_mutex.lock();
			auto iter = ::std::map<KEY,VAL>::find(key_);
			bool ret = iter != ::std::map<KEY, VAL>::end();
			if (lock)
				this->m_mutex.unlock();
			return ret;
		}
		bool set(const KEY& key_, VAL val, bool insert = false)
		{
			____lock____(this->m_mutex, m_lock);
			typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY, VAL>::find(key_);
			if (iter == ::std::map<KEY, VAL>::end())
			{
				if (insert == true)
				{
					::std::map<KEY, VAL>::insert(std::pair<KEY, VAL>(key_, val));
					return true;
				}
				else
				{
					return false;
				}

			}
			else
			{
				iter->second = val;
				return true;
			}
		}
		bool get(const KEY& key_, VAL& val, bool lock = true)
		{
			if (lock)
				this->m_mutex.lock();


			typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY, VAL>::find(key_);
			if (iter == ::std::map<KEY, VAL>::end()) {
				if (lock)
					this->m_mutex.unlock();
				return false;
			}

			val = iter->second;
			if (lock)
				this->m_mutex.unlock();
			return true;
		}
		bool del(const KEY& key_,bool locked = true)
		{
			____lock____(this->m_mutex, locked);
			typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY, VAL>::find(key_);
			if (iter == ::std::map<KEY, VAL>::end())
				return false;
			::std::map<KEY, VAL>::erase(iter);
			return true;
		}
		void clear()
		{
			____lock____(this->m_mutex, m_lock);
			//::std::map<KEY,VAL>::swap();
			::std::map<KEY,VAL>::clear();
		}
		uint32 size()
		{
			____lock____(this->m_mutex, m_lock);
			return ::std::map<KEY,VAL>::size();
		}
		void lock()
		{
			this->m_mutex.lock();
		}
		void unlock()
		{
			this->m_mutex.unlock();
		}
		VAL operator[](const KEY& key)
		{
			____lock____(this->m_mutex, m_lock);
			VAL val;
			get(key, val,false);
			return val;
		}
		newobj::vector<VAL> to_vector()
		{
			____lock____(this->m_mutex, m_lock);
			newobj::vector<VAL> result;
			for_iter(iter, (*this))
				result.push_back(iter->second);
			return result;
		}
		bool find(std::function<bool(const KEY& key, const VAL& value)> delegate)
		{
			____lock____(this->m_mutex, m_lock);
			for_iter(iter, (*this))
			{
				if (delegate(iter->first, iter->second))
					return true;
			}
			return false;
		}
		void loop(std::function<void(const KEY& key,const VAL& value)> callback)
		{
			____lock____(this->m_mutex, m_lock);
			//____lock____(this->m_mutex,true);
			for_iter(iter, (*this))
			{
				callback(iter->first,iter->second);
			}
		}
		std::map<KEY, VAL>* parent()
		{
			return this;
		}
	public:
		newobj::mutex m_mutex;
		bool m_lock;
	};
}

