#pragma once
#include <vector>
#include "basetype/string.h"
#include "public/struct.h"
#include "util/json.h"
#ifdef QT_CORE_LIB
#include "QtCore/qstring.h"
#endif

namespace newobj
{
	namespace forstring
	{
			
			/************************************************************************
			*	Function: 转换为string对象
			************************************************************************/
			nstring  NEWOBJ_API to_stringf(float number, uint32 bl = -1);
			nstring  NEWOBJ_API to_stringf(double number, uint32 bl = -1);

			/************************************************************************
			 *	Function: 英文转大小写转换
			 * Param
			 *			to_big				:			转换为大写(true=转为大写，false=转为小写)
			 ************************************************************************/
			nstring NEWOBJ_API en_case(const nstring& src,bool to_big);


			/************************************************************************
			 *	Function: nstring、std::wstring互转
			 ************************************************************************/
			std::wstring NEWOBJ_API to_wstring(const nstring& str,size_t encode = 65001/*UTF8*/);
			nstring NEWOBJ_API to_string(const std::wstring& wstr, size_t encode = 65001/*UTF8*/);

			/*
				添补尾部
				@value					：			源数据
				@last_size			：			填补后的数据长度
				@fill						：			添补数据
			*/
			nstring NEWOBJ_API replenish_end(const nstring& value,uint32 max_size,char fill);
			nstring NEWOBJ_API replenish_begin(const nstring& value, uint32 max_size, char fill);


            nstring NEWOBJ_API from_hex(const nstring& hex);
            nstring NEWOBJ_API to_hex(const nstring& value);

			/************************************************************************
			 *	Function: JSON无子集，A-Z排序
			 ************************************************************************/
			//std::vector<Kv> json_az(const newobj::json& value);
	}
}
