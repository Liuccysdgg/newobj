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
	 * Class: 异常处理类
	 ****************************************************************/
	class NEWOBJ_API exception:public std::exception
	{
	public:
		/****************************************************************
		 * Fun: 构造函数
		 * Param
		 *			e								:		异常描述
		 ****************************************************************/
		exception(const nstring& e = nstring(""));
		/****************************************************************
		 * Fun: operator +=
		 * Param
		 *			e								:		异常描述,用于追加
		 ****************************************************************/
		void operator+=(const nstring& e);
		/****************************************************************
		 * Fun: 打印所有异常
		 ****************************************************************/
		void print() const;
		/****************************************************************
		 * Fun: 打印并中止程序
		 ****************************************************************/
		void abort() const;
		/****************************************************************
		 * Fun: 设置代码地址
		 * Param
		 *			line							:		代码行
		 *			filename					:		文件名
		 *			funname					:		函数名
		 ****************************************************************/
		void set_codeaddr(uint32 line,const nstring&filename,const nstring&funname);
		/****************************************************************
		 * Fun: 获取错误描述数组
		 ****************************************************************/
		const std::vector<nstring>& errors() const;
		/****************************************************************
		 * Fun: 获取错误发生文件名
		 ****************************************************************/
		const nstring& filename() const;
		/****************************************************************
		 * Fun: 获取错误发生函数名
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
		std::vector<nstring> m_es;			//异常描述数组
		uint32 m_code_line;				//异常代码行
		nstring m_code_filename;				//异常代码文件名
		nstring m_code_funname;				//异常代码函数名
		ptr m_log_ptr;
	};
}

