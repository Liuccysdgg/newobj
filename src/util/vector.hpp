#pragma once
#include <vector>
#include <functional>
#include "basetype/string.h"
#include "public/exception.h"
namespace newobj
{
    template<typename T>
    struct nolock_array
    {
		nolock_array() {
			m_array = nullptr;
			m_count = 0;
		}
        ~nolock_array(){
            free();
        }
        void free(){
            if(m_array != nullptr)
                delete[] m_array;
            m_array = nullptr;
            m_count = 0;
        }
        void init(const std::vector<T>& value){
            if(value.size() == 0)
                return;
            m_array = new T[value.size()];
            m_count = value.size();
            for(size_t i=0;i<value.size();i++){
                m_array[i] = value[i];
            }
        }
        size_t append(const T& value){
            std::vector<T> v;
            v.resize(m_count+1);
            for(size_t i=0;i<m_count;i++){
                 v[i] = m_array[i];
            }
            v[m_count] = value;
            free();
            init(v);
            return m_count-1;
        }
        inline T get(size_t index){
            if(index >= m_count){
                printf("Unlocked array index is too long");
                abort();
            }
             return m_array[index];
        }

        T* m_array;
        size_t m_count;
    };


	template<typename T>
	class vector:public std::vector<T>
	{
	public:
		vector<T>(const std::vector<T>& value):std::vector<T>()
		{
			operator=(value);
		}
		vector<T>() : std::vector<T>()
		{

		}
		~vector<T>()
		{

		}
		void operator=(const std::vector<T>& value)
		{
			*((std::vector<T>*)this) = value;
		}
		bool find(std::function<bool(T& value, uint32 idx)> callback)
		{
			for (uint32 i = 0; i < this->size(); i++)
			{
				if (callback((*this)[i], i))
					return true;
			}
			return false;
		}
		bool find(const T& value)
		{
			for (uint32 i = 0; i < this->size(); i++)
			{
				if(value == this->at(i))
					return true;
			}
			return false;
		}
		bool find(std::function<bool(T& value, uint32 idx)> callback,T& value)
		{
			for (uint32 i = 0; i < this->size(); i++)
			{
				if (callback((*this)[i], i))
				{
					value =this->at(i);
					return true;
				}
			}
			return false;
		}
		void loop(std::function<void(const T& value)> callback)
		{
			for (uint32 i = 0; i < this->size(); i++)
				callback(this->at(i));
		}
		void loop_fl(std::function<void(const T& value,bool first,bool last)> callback)
		{
			for (uint32 i = 0; i < this->size(); i++)
				callback(this->at(i),i==0,i==this->size()-1);
		}
		void rloop(std::function<void(const T& value)> callback)
		{
			uint32 size = this->size();
			for (uint32 i = 0; i < this->size(); i++)
				callback(this->at(size-i-1));
		}
		void rloop_fl(std::function<void(const T& value, bool first, bool last)> callback)
		{
			uint32 size = this->size();
			for (uint32 i = 0; i < this->size(); i++)
				callback(this->at(size - i - 1), i == size - 1, i == 0);
		}
	};
}
