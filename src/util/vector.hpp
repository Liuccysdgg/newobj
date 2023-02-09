#pragma once
#include <vector>
#include <functional>
namespace newobj
{
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