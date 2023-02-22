#pragma once
#include <string>
#include "public/define.h"
#include "basetype/string.h"
namespace newobj
{
		namespace verify
		{
			/*********************************************************
			*	Function : �ж��ַ��Ƿ�Ϊ����
			*********************************************************/
			bool NEWOBJ_API is_num(char code);
			/*********************************************************
			*	Function : �ж��ַ����Ƿ�Ϊ����
			*********************************************************/
			bool NEWOBJ_API  is_nums(const nstring& str);
			/*********************************************************
			*	Function : �ж������ַ����Ƿ����
			*********************************************************/
			bool  NEWOBJ_API equal_en(const nstring& str1, const nstring str2);
			/*********************************************************
			*	Function : �ж��Ƿ�ΪӢ���ַ�
			*********************************************************/
			bool NEWOBJ_API  is_en(char code);
			/*********************************************************
			*	Function : �ж��Ƿ�����Ӣ����ĸ����ȣ������ִ�Сд
			*********************************************************/
			bool  NEWOBJ_API equal_en(char code1, char code2);
			bool  NEWOBJ_API is_mobile(const nstring& mobile);
			/*�Ƿ�Ϊ�������*/
			bool  NEWOBJ_API is_asc(char code);
			bool  NEWOBJ_API is_pwd(const nstring& password);
			/*��������ַ���*/
			bool  NEWOBJ_API is_a(const nstring& str);
		}
}