#pragma once
#include "public/define.h"
#include <vector>
#include <map>
#include "util/vector.hpp"
/*
 * �ṹ��vector int ��Ա����
 */
#define SORT_VECTOR_MEMBER(VECTOR,MEMBER,PS)																											\
	{																																																\
		for (uint32 i = 0; i < VECTOR.size(); i++)																																\
		{																																															\
			for (uint32 x = i; x < VECTOR.size(); x++)																														\
			{																																														\
				if (PS == true ? VECTOR[i].MEMBER > VECTOR[x].MEMBER:VECTOR[i].MEMBER < VECTOR[x].MEMBER)				\
				{																																													\
					auto temp = VECTOR[i];																																			\
					VECTOR[i] = VECTOR[x];																																			\
					VECTOR[x] = temp;																																					\
				}																																													\
			}																																														\
		}																																															\
	}
namespace newobj
{
	namespace stl
	{
		/*
		*	����
		*	@ps			��			����
		*/
		inline void sort(std::vector<int>& value,bool ps)
		{
			int temp = 0;
			for (size_t i = 0; i < value.size(); i++)
			{
				for (size_t x = i; x < value.size(); x++)
				{
					if (ps==true?value[i] > value[x]:value[i] < value[x])
					{
						temp = value[i];
						value[i] = value[x];
						value[x] = temp;
					}
				}
			}
		}
		/*
		*	����
		*	@ps			��			����
		*/
		template<typename T>
		inline void sort(std::vector<int>& value,std::vector<T> &extra, bool ps)
		{
			int temp = 0;
			for (uint32 i = 0; i < value.size(); i++)
			{
				for (uint32 x = i; x < value.size(); x++)
				{
					if (ps == true ? value[i] > value[x]:value[i] < value[x])
					{
						temp = value[i];
						value[i] = value[x];
						value[x] = temp;

						auto t = extra[i];
						extra[i] = extra[x];
						extra[x] = t;
					}
				}
			}
		}
		/*
		 * ��ҳר��
		 * @ps		��		����
		 */
		template<typename KEY, typename VAL>
		inline std::map<KEY, VAL> limit(const std::map<KEY, VAL>& value,uint32 start,uint32 length,bool ps)
		{
			std::map<KEY, VAL> result;
			uint32 idx = 0;
			if (ps)
			{
				for_iter(iter, value)
				{
					if (idx >= start && idx < start + length)
						result.insert(std::pair<KEY, VAL>(iter->first, iter->second));
					else if (idx > start + length)
						break;
					idx++;
				}
			}
			else
			{
				for_riter(iter, value)
				{
					if (idx >= start && idx < start + length)
						result.insert(std::pair<KEY, VAL>(iter->first, iter->second));
					else if (idx > start + length)
						break;
					idx++;
				}
			}
			

			return result;
		}
		template<typename T>
		inline std::vector<T> limit(const std::vector<T>& value, uint32 start, uint32 length, bool ps)
		{
			std::vector<T> result;
			uint32 idx = 0;
			if (ps)
			{
				for_iter(iter, value)
				{
					if (idx >= start && idx < start + length)
						result.push_back(*iter);
					else if (idx > start + length)
						break;
					idx++;
				}
			}
			else
			{
				for_riter(iter, value)
				{
					if (idx >= start && idx < start + length)
						result.push_back(*iter);
					else if (idx > start + length)
						break;
					idx++;
				}
			}
			

			return result;
		}
		/*
		 * std::mapתstd::vector
		 */
		template<typename KEY,typename VAL>
		inline std::vector<VAL> to_vector_val(const std::map<KEY, VAL>& value)
		{
			std::vector<VAL> result;
			for_iter(iter, value)
				result.push_back(iter->second);
			return result;
		}


	}
}