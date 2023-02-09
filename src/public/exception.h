#pragma once
#include "define.h"
#include <string>
#include <vector>
#include "util/forstring.h"
#include "string.h"
#include <stdexcept>
namespace newobj
{

	/****************************************************************
	 * Class: �쳣������
	 ****************************************************************/
	class NEWOBJ_API exception:public std::exception
	{
	public:
		/****************************************************************
		 * Fun: ���캯��
		 * Param
		 *			e								:		�쳣����
		 ****************************************************************/
		exception(const nstring& e = nstring(""));
		/****************************************************************
		 * Fun: operator +=
		 * Param
		 *			e								:		�쳣����,����׷��
		 ****************************************************************/
		void operator+=(const nstring& e);
		/****************************************************************
		 * Fun: ��ӡ�����쳣
		 ****************************************************************/
		void print() const;
		/****************************************************************
		 * Fun: ��ӡ����ֹ����
		 ****************************************************************/
		void abort() const;
		/****************************************************************
		 * Fun: ���ô����ַ
		 * Param
		 *			line							:		������
		 *			filename					:		�ļ���
		 *			funname					:		������
		 ****************************************************************/
		void set_codeaddr(uint32 line,const nstring&filename,const nstring&funname);
		/****************************************************************
		 * Fun: ��ȡ������������
		 ****************************************************************/
		const std::vector<nstring>& errors() const;
		/****************************************************************
		 * Fun: ��ȡ�������ļ���
		 ****************************************************************/
		const nstring& filename() const;
		/****************************************************************
		 * Fun: ��ȡ������������
		 ****************************************************************/
		const nstring& funname() const;
		void set_logptr(ptr log);
		nstring to_string() const;
		/*	what fuck !!!	?*/
		virtual char const* what() const 
#ifndef _WIN32
			noexcept 
#endif
			override;
	private:
		std::vector<nstring> m_es;			//�쳣��������
		uint32 m_code_line;				//�쳣������
		nstring m_code_filename;				//�쳣�����ļ���
		nstring m_code_funname;				//�쳣���뺯����
		ptr m_log_ptr;
	};
}

