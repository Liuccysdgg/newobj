#include "log/log_no.h"
#include "util/print.h"
#include "public/exception.h"
#include <string.h>
newobj::log::no::no(const nstring& dirpath, bool only_filepath, bool print):slice(dirpath, only_filepath==true?newobj::log::slice::ST_ONLY_FILE:newobj::log::slice::ST_DAY,"%Y-%m-%d.log")
{
	m_path = dirpath;
	m_print = print;
}

newobj::log::no::~no()
{
}

void newobj::log::no::print(bool param)
{
	m_print = param;
}

bool newobj::log::no::print()
{
	return m_print;
}

nstring newobj::log::no::make(const nstring& type, const nstring& value, const nstring& location_code_info, int32 color, int32 color_bg, int32 attr)
{
	char line_type[64];
	char line_time[64];
	char line_location[256];
	char line_content[8192];

	memset(line_type, 0, 64);
	memset(line_time, 0, 64);
	memset(line_location, 0, 256);
	memset(line_content, 0, 8192);

	if(type == "succ")
		newobj::print::com_str(line_type,64, "[" +type  + "]\t",CONSOLE_GREEN, CONSOLE_BG_BLACK, 0);
	else if (type == "gen")
		newobj::print::com_str(line_type, 64, "[" + type + "]\t", CONSOLE_CYANBLUE, CONSOLE_BG_BLACK, 0);
	else if (type == "warn")
		newobj::print::com_str(line_type, 64, "[" + type + "]\t", CONSOLE_YELLOW, CONSOLE_BG_BLACK, 0);
	else if (type == "error" || type == "exp")
	{
		newobj::print::com_str(line_type, 64, "[" + type + "]\t", CONSOLE_RED, CONSOLE_BG_BLACK, 0);
		newobj::print::com_str(line_location, 256, location_code_info+"\t", CONSOLE_PURPLERED, CONSOLE_BG_BLACK, 0);
	}
		


	newobj::print::com_str(line_time,64, time::now_time()+" ", CONSOLE_WHITE, CONSOLE_BG_BLACK, 0);
	newobj::print::com_str(line_content,8192, value, color, color_bg, attr);

	if (m_print)
	{
		if (type == "error" || type == "exp")
			printf("%s%s%s%s\n", line_type, line_time, line_location, line_content);
		else
			printf("%s%s%s\n", line_type, line_time, line_content);
		
	}
	nstring result;
	result.append("[" + type + "] ");
	result.append(time::now_time() + " ");
	result.append(value+"\r\n");
	return result;
}

newobj::log::no& newobj::log::no::succ(const nstring& value)
{
	write(make("succ", value, "", CONSOLE_GREEN, CONSOLE_BG_BLACK, 0));
	return *this;
}
newobj::log::no& newobj::log::no::warn(const nstring& value)
{
	// TODO: �ڴ˴����� return ���
	write(make("warn", value, "", CONSOLE_YELLOW, CONSOLE_BG_BLACK, 0));
	return *this;
}

newobj::log::no& newobj::log::no::info(const nstring& value)
{
	// TODO: �ڴ˴����� return ���
	write(make("info", value, "", CONSOLE_CYANBLUE, CONSOLE_BG_BLACK, 0));
	return *this;
}
newobj::log::no& newobj::log::no::error(const nstring& value, const nstring& location_code_info)
{
	write(make("error", value, location_code_info, CONSOLE_RED, CONSOLE_BG_BLACK, 0));
	return *this;
}
void newobj::log::no::exp(const nstring& value, const nstring& location_code_info)
{
	write(make("exp", value, location_code_info, CONSOLE_RED, CONSOLE_BG_BLACK, 0));
	trw_str(value);
}