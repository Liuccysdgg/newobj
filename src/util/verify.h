#pragma once
#include <string>
#include "public/define.h"
#include "basetype/string.h"
namespace newobj
{
		namespace verify
		{
			/*********************************************************
			*	Function : 判断字符是否为数字
			*********************************************************/
			bool NEWOBJ_API is_num(char code);
			/*********************************************************
			*	Function : 判断字符串是否为数字
			*********************************************************/
			bool NEWOBJ_API  is_nums(const nstring& str);
			/*********************************************************
			*	Function : 判断两个字符串是否相等
			*********************************************************/
			bool  NEWOBJ_API equal_en(const nstring& str1, const nstring str2);
			/*********************************************************
			*	Function : 判断是否为英文字符
			*********************************************************/
			bool NEWOBJ_API  is_en(char code);
			/*********************************************************
			*	Function : 判断是否两个英文字母与相等，不区分大小写
			*********************************************************/
			bool  NEWOBJ_API equal_en(char code1, char code2);
			bool  NEWOBJ_API is_mobile(const nstring& mobile);
			/*是否为特殊代码*/
			bool  NEWOBJ_API is_asc(char code);
			bool  NEWOBJ_API is_pwd(const nstring& password);
			/*特殊代码字符串*/
			bool  NEWOBJ_API is_a(const nstring& str);
		}
}