#pragma once
#include <string>
#include "public/define.h"
#include "basetype/string.h"
#include "public/buffer.h"
namespace newobj
{
	namespace bytes
	{

		/*�����ֽ�*/
		bool NEWOBJ_API search(const char* src_buf, uint32 src_len, const char* search_buf, uint32 search_len);
		/*ȡλ*/
		uchar NEWOBJ_API bit32(uint32 num, int pos); 
		uchar NEWOBJ_API bit16(ushort num, int pos);
		uchar NEWOBJ_API bit8(uchar num, int pos);
		/*��λ*/
		void NEWOBJ_API bit64(uint64 &num, int pos, uchar bit);
		void NEWOBJ_API bit32(uint32 &num, int pos, uchar bit);
		void NEWOBJ_API bit16(ushort &num, int pos, uchar bit);
		void NEWOBJ_API bit8(uchar &num, int pos, uchar bit);

		/*תʮ�������ַ���*/
		nstring to_hexstring(const ushort hex);
		nstring to_hexstring(const int32 dec);

		/************************************************************************
		 *	Function: ʮ������תint32
		 ************************************************************************/
		uint32 NEWOBJ_API  hex_to_uint32(const nstring& hex_str);
		/************************************************************************
		 *	Function: ʮ������תuint64
		 ************************************************************************/
		uint64  NEWOBJ_API hex_to_uint64(const nstring& hexsrc);
		/*��ת*/
		void NEWOBJ_API to_int(int32& dest, const char* src, bool reverse = false);
		void NEWOBJ_API to_uint(uint32& dest, const char* src, bool reverse = false);
		void NEWOBJ_API to_short(short& dest, const char* src, bool reverse = false);
		void NEWOBJ_API to_ushort(ushort& dest, const char* src, bool reverse = false);
		void NEWOBJ_API to_char(char* dest, short src, bool reverse = false);
		void NEWOBJ_API to_char(char* dest, int32 src, bool reverse = false);
		void NEWOBJ_API to_char(char* dest, int64 src, bool reverse = false);
		newobj::buffer NEWOBJ_API to_buffer(short src, bool reverse = false);
		newobj::buffer NEWOBJ_API to_buffer(int32 src, bool reverse = false);
		newobj::buffer NEWOBJ_API to_buffer(int64 src, bool reverse = false);
		newobj::buffer NEWOBJ_API to_buffer(uchar src);

		void NEWOBJ_API to_uint64(uint64& dest, const char* src, bool reverse = false);
		void NEWOBJ_API to_int64(int64& dest, const char* src, bool reverse = false);
		nstring NEWOBJ_API to_string(const char* src, bool reverse = false);



		/*Ansi To unicode*/
		newobj::buffer to_unicode(const newobj::buffer& data);
	}
}
