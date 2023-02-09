#pragma once
#include "util/forstring.h"
#include "public/define.h"
#include "public/string.h"
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

// �»���
#define CONSOLE_ATTR_UNDERLINE 4

namespace newobj
{
		namespace print
		{
			/*��ӡ����ɫ���ַ���-������*/
			void NEWOBJ_API str_ln(const nstring& content, int color, int bg, int attr);
			/*�ϳɴ���ɫ���ַ���*/
			void NEWOBJ_API com_str(char* line, uint32 max_length, nstring content, int color, int bg, int attr);

			/*��ӡ����ɫ���ַ���*/
			void NEWOBJ_API str(const nstring& content, int color, int bg, int attr);
			/*�����׼������*/
			void NEWOBJ_API print_xy(const nstring& content, int x, int y, int color, int bg, int attr);
			/*�����׼������*/
			void NEWOBJ_API print_xy(const nstring& content,int x,int y);
			/*��ӡʮ����������*/ 
			void NEWOBJ_API print_hex(const char * data,uint32 len);
			/*��ӡʮ����������*/
			void NEWOBJ_API print_hex(const newobj::buffer& data);
			/*�ַ�תʮ������*/
			nstring NEWOBJ_API charToHex(char data);
		}
}