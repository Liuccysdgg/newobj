#pragma once
#include "util/forstring.h"
#include "public/define.h"
#include "basetype/string.h"
#include "public/buffer.h"
#define CONSOLE_RESET "\033[0;0;0m"

#define CONSOLE_BLACK 30
#define CONSOLE_RED 31
#define CONSOLE_GREEN 32
#define CONSOLE_YELLOW 33
#define CONSOLE_BLUE 34
#define CONSOLE_PURPLERED 35
#define CONSOLE_CYANBLUE 36
#define CONSOLE_WHITE 37

#define CONSOLE_BG_BLACK 40
#define CONSOLE_BG_RED 41
#define CONSOLE_BG_GREEN 42
#define CONSOLE_BG_YELLOW 43
#define CONSOLE_BG_BLUE 44
#define CONSOLE_BG_PURPLERED 45
#define CONSOLE_BG_CYANBLUE 46
#define CONSOLE_BG_WHITE 47

// 下划线
#define CONSOLE_ATTR_UNDERLINE 4

namespace newobj
{
		namespace print
		{
			/*打印带颜色的字符串-带换行*/
			void NEWOBJ_API str_ln(const nstring& content, int color, int bg, int attr);
			/*合成带颜色的字符串*/
			void NEWOBJ_API com_str(char* line, uint32 max_length, nstring content, int color, int bg, int attr);

			/*打印带颜色的字符串*/
			void NEWOBJ_API str(const nstring& content, int color, int bg, int attr);
			/*输出标准带坐标*/
			void NEWOBJ_API print_xy(const nstring& content, int x, int y, int color, int bg, int attr);
			/*输出标准带坐标*/
			void NEWOBJ_API print_xy(const nstring& content,int x,int y);
			/*打印十六进制数据*/ 
			void NEWOBJ_API print_hex(const char * data,uint32 len);
			/*打印十六进制数据*/
			void NEWOBJ_API print_hex(const newobj::buffer& data);
			/*字符转十六进制*/
			nstring NEWOBJ_API charToHex(char data);
		}
}