#include "print.h"
#include <string.h>
#include <iostream>
#ifdef  _WIN32
#include <Windows.h>
#endif
void newobj::print::str_ln(const nstring& content, int color, int bg, int attr)
{
	str(content, color, bg, attr);
	printf("\n");
}
void newobj::print::com_str(char* line,uint32 max_length,nstring content, int color, int bg, int attr)
{
	if (content.length() > max_length)
		content = content.substr(0, max_length);
#if ((defined  _WIN32) && (defined _DEBUG) || (!defined _WIN32))
#ifdef _WIN32
	 SPRINTF(line, max_length,"\33[%d;%d;%dm%s\33[0;0;0m", attr, color, bg, content.c_str());
#else
	SPRINTF(line, "\33[%d;%d;%dm%s\33[0;0;0m", attr, color, bg, content.c_str());
#endif
#else
#ifdef _WIN32
	strcpy_s(line, 8192,content.c_str());
#else
	strcpy(line, content.c_str());
#endif
#endif
}

void newobj::print::str(const nstring& content, int color, int bg, int attr)
{
#if ((defined  _WIN32) && (defined _DEBUG) || (!defined _WIN32))
	printf("\33[%d;%d;%dm%s\33[0;0;0m", attr, color, bg, content.c_str());
#else
	printf("%s", content.c_str());
#endif
}

void newobj::print::print_xy(const nstring& content,int x,int y,int color, int bg, int attr)
{
#if (defined  _WIN32)
	HANDLE hd;
	COORD pos;

	pos.X = x;
	pos.Y = y;
	hd = GetStdHandle(STD_OUTPUT_HANDLE);	/*获取标准输出的句柄*/
	SetConsoleCursorPosition(hd, pos);
	printf("\33[%d;%d;%dm%s\33[0;0;0m", attr, color, bg, content.c_str());
#else
	printf("%s", content.c_str());
#endif
}

void newobj::print::print_xy(const nstring& content, int x, int y)
{
#if (defined  _WIN32)
	HANDLE hd;
	COORD pos;

	pos.X = x;
	pos.Y = y;
	hd = GetStdHandle(STD_OUTPUT_HANDLE);	/*获取标准输出的句柄*/
	SetConsoleCursorPosition(hd, pos);
	printf("%s",content.c_str());
#else
	printf("%s", content.c_str());
#endif
}

void  newobj::print::print_hex(const char* data, uint32 len)
{
	char new_buf_ptr[20];
	for (uint32 i = 0; i < len; i++)
	{
		memset(new_buf_ptr, 0, 20);
#ifdef _WIN32
		SPRINTF(new_buf_ptr, "[%1X]", (char)*(data + i));
#else
		SPRINTF(new_buf_ptr, "[%1X]", (char)*(data + i));
#endif
		if (new_buf_ptr[0] == '\0')
		{
			abort();
		}
		if (strlen(new_buf_ptr) > 4)
		{
			new_buf_ptr[1] = new_buf_ptr[7];
			new_buf_ptr[2] = new_buf_ptr[8];
			new_buf_ptr[3] = ']';
			new_buf_ptr[4] = '\0';
		}

		newobj::print::str(new_buf_ptr, CONSOLE_WHITE, CONSOLE_BG_BLUE, 0);
	}
	printf("\n");
}

void  newobj::print::print_hex(const newobj::buffer& data)
{
	print_hex(data.data(), data.length());
}

nstring  newobj::print::charToHex(char data)
{
	char new_buf_ptr[20];
	memset(new_buf_ptr, 0, 20);
#ifdef _WIN32
	SPRINTF(new_buf_ptr, "%1X", (char)*(&data));
#else
	SPRINTF(new_buf_ptr, "%1X", (char)*(&data));
#endif
	if (new_buf_ptr[0] == '\0')
	{
		abort();
	}
	if (strlen(new_buf_ptr) > 2)
	{
		new_buf_ptr[0] = new_buf_ptr[6];
		new_buf_ptr[1] = new_buf_ptr[7];
		new_buf_ptr[2] = '\0';
	}
	nstring ret = new_buf_ptr;
	if (ret.length() == 1)
		ret = "0" + ret;

	return ret;
}
