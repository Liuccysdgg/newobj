#include "exception.h"
#include <iostream>
#include <string>
#include "util/forstring.h"

newobj::exception::exception(const nstring& e)
{
	this->m_code_line = 0;
	this->m_log_ptr = 0;
	if (!e.empty())
	{
		*this += e;
	}

}

void newobj::exception::operator+=(const nstring& e)
{
	this->m_es.push_back(e);
}

void newobj::exception::print() const
{
	nstring print_str;
	print_str = "\n*****************[exception]**********************\n";
	if (this->m_code_line != 0)
	{
		print_str.append("FileName: \t" + this->m_code_filename+"\n");
		print_str.append("FunName: \t" + this->m_code_funname + "\n");
		print_str.append("Line: \t\t"+ nstring::from(this->m_code_line) +"\n");
	}
	print_str.append("Desc: \n");
	for (uint32 i = 0; i < this->m_es.size(); i++)
		print_str.append(" [" + nstring::from(i) + "]: " + this->m_es[i] + "\n");
	print_str.append("**************************************************\n");

	//newobj::log::error(0, print_str, CURR_LOCATION);
	//LOG_ERROR(m_log_ptr,print_str);
}

void newobj::exception::abort() const
{
	print();
	::abort();
}

void newobj::exception::set_codeaddr(uint32 line, const nstring& filename, const nstring& funname)
{

	this->m_code_line = line;
	this->m_code_filename = filename;
	this->m_code_funname = funname;
}

const std::vector<nstring>& newobj::exception::errors() const
{
	// TODO: 在此处插入 return 语句
	return this->m_es;
}

const nstring& newobj::exception::filename() const
{
	// TODO: 在此处插入 return 语句
	return this->m_code_filename;
}

const nstring& newobj::exception::funname() const
{
	// TODO: 在此处插入 return 语句
	return this->m_code_funname;
}

void newobj::exception::set_logptr(ptr log)
{
	this->m_log_ptr = log;
}

nstring newobj::exception::to_string() const
{
	nstring print_str;
	for (uint32 i = 0; i < this->m_es.size(); i++)
		print_str.append(" [" + nstring::from(i) + "]: " + this->m_es[i] + "\n");
	return print_str;
	/*nstring print_str;
	print_str = "\n*****************[exception]**********************\n";
	if (this->m_code_line != 0)
	{
		print_str.append("FileName: \t" + this->m_code_filename + "\n");
		print_str.append("FunName: \t" + this->m_code_funname + "\n");
		print_str.append("Line: \t\t" + nstring::from(this->m_code_line) + "\n");
	}
	print_str.append("Desc: \n");
	for (uint32 i = 0; i < this->m_es.size(); i++)
		print_str.append(" [" + nstring::from(i) + "]: " + this->m_es[i] + "\n");
	print_str.append("**************************************************\n");
	return print_str;*/
}
char const* newobj::exception::what() const 
#ifndef _WIN32
noexcept
#endif
{
	static nstring result = to_string();
	this->print();
	return result.c_str();
}
