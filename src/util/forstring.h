#pragma once
#include <vector>
#include "public/string.h"
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
			*	Function: ת��Ϊstring����
			************************************************************************/
			nstring  NEWOBJ_API to_stringf(float number, uint32 bl = -1);
			nstring  NEWOBJ_API to_stringf(double number, uint32 bl = -1);

			/************************************************************************
			 *	Function: Ӣ��ת��Сдת��
			 * Param
			 *			to_big				:			ת��Ϊ��д(true=תΪ��д��false=תΪСд)
			 ************************************************************************/
			nstring NEWOBJ_API en_case(const nstring& src,bool to_big);


			/************************************************************************
			 *	Function: nstring��std::wstring��ת
			 ************************************************************************/
			std::wstring NEWOBJ_API to_wstring(const nstring& str,size_t encode = 65001/*UTF8*/);
			nstring NEWOBJ_API to_string(const std::wstring& wstr, size_t encode = 65001/*UTF8*/);

			/*
				��β��
				@value					��			Դ����
				@last_size			��			�������ݳ���
				@fill						��			������
			*/
			nstring NEWOBJ_API replenish_end(const nstring& value,uint32 max_size,char fill);
			nstring NEWOBJ_API replenish_begin(const nstring& value, uint32 max_size, char fill);



			/************************************************************************
			 *	Function: JSON���Ӽ���A-Z����
			 ************************************************************************/
			//std::vector<Kv> json_az(const newobj::json& value);
	}
}
