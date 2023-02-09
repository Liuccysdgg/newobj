#include "log_slice.h"
#include "util/codec.h"
#include "util/system.h"
bool CreateNewFile(file* file, const nstring& filepath)
{
	file->close();
    return file->open(newobj::file::format_slash(filepath));
};
newobj::log::slice::slice(const nstring& dirpath, slice::Type type, const nstring& format)
{
	m_type = type;
	m_format = format;
	m_dirpath = dirpath;
	if (type == slice::ST_ONLY_FILE)
	{
		CreateNewFile(&m_file,dirpath);
	}
}

newobj::log::slice::~slice() 
{
}

bool newobj::log::slice::write(const nstring& txt) 
{
	____lock____(this->m_mutex,true);
	if (m_type == ST_ONLY_FILE)
	{

	}
	else
	{
		auto now = time::now_time2();
		auto now_str = time::now_time(m_format);

		if (m_pre_date.year == 0)
		{
			m_pre_date = now;
			f_ret_f(CreateNewFile(&m_file, m_dirpath + "\\" + now_str));
		}
		if (m_type == ST_YEAR)
		{
			if (m_pre_date.year != now.year)
			{
				f_ret_f(CreateNewFile(&m_file, m_dirpath + "\\" + now_str));
			}
		}
		else if (m_type == ST_MONTH)
		{
			if (m_pre_date.year != now.year || m_pre_date.month != now.month)
			{
				f_ret_f(CreateNewFile(&m_file, m_dirpath + "\\" + now_str));
			}
		}
		else if (m_type == ST_DAY)
		{
			if (m_pre_date.year != now.year || m_pre_date.month != now.month || m_pre_date.day != now.day)
			{
				f_ret_f(CreateNewFile(&m_file, m_dirpath + "\\" + now_str));
			}
		}
		else if (m_type == ST_HOUR)
		{
			if (m_pre_date.year != now.year || m_pre_date.month != now.month || m_pre_date.day != now.day || m_pre_date.hour != now.hour)
			{
				f_ret_f(CreateNewFile(&m_file, m_dirpath + "\\" + now_str));
			}

		}
	}
	
	return m_file.write(txt.c_str(),txt.length()) && m_file.flush();
}
