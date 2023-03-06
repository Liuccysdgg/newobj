#include "forstring.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "public/buffer.h"

namespace newobj
{
	namespace forstring
	{
		nstring to_stringf(float number, uint32 bl)
		{
			return to_stringf((double)number, bl);
		}
		nstring to_stringf(double number, uint32 bl)
		{
			nstring x = nstring::from(number);
			while (x.length() != 1)
			{
				if (x[x.length() - 1] == '.')
					break;
				else if (x[x.length() - 1] == '0')
					x = x.substr(0, x.length() - 1);
				else
					break;
			}
			int find = x.find(".");
			if (find < 0)
				return x;
			if (find + bl + 1 <= x.length())
				x = x.substr(0, find + bl + 1);
			return x;
		}
		
		nstring  en_case(const nstring& src, bool toBig)
		{
			buffer buf(src);
			for (size_t i = 0; i < buf.length(); i++)
			{
				if (toBig)
				{
					if (buf[i] >= 97 && buf[i] <= 122)
						buf[i] -= 32;
				}
				else
				{
					if (buf[i] >= 65 && buf[i] <= 90)
						buf[i] += 32;
				}
			}
			return nstring(buf.data(), (uint32)buf.length());
		}
#ifdef _WIN32
		std::wstring to_wstring(const nstring& str, size_t encode)
		{
			int len;
			int slength = (int)str.length() + 1;
			len = MultiByteToWideChar(encode, 0, str.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(encode, 0, str.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
		}

		nstring to_string(const std::wstring& wstr, size_t encode)
		{
			int len = WideCharToMultiByte(encode, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
			char* buffer = new char[len + 1];
			memset(buffer, '\0', sizeof(char) * (len + 1));
			WideCharToMultiByte(encode, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
			nstring result(buffer);
			delete[] buffer;
			return result;
		}
#endif
		nstring replenish_end(const nstring& value, uint32 max_size, char fill)
		{
			nstring result = value;
			for (uint32 i = value.length(); i < max_size; i++)
				result.append(&fill, 1);
			return result;
		}
		nstring replenish_begin(const nstring& value, uint32 max_size, char fill)
		{
			nstring result = value;
			for (uint32 i = value.length(); i < max_size; i++)
				result = nstring(&fill, 1) + result;
			return result;
		}
        nstring NEWOBJ_API from_hex(const nstring& hex)
        {
            nstring result;
            for (size_t i = 0; i < hex.length(); i += 2)
            {
                nstring byte = hex.substr(i, 2);
                char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
                result.append(chr);

            }
            return result;
        }
        nstring NEWOBJ_API to_hex(const nstring& value)
        {
            static const char* const lut = "0123456789ABCDEF";
            size_t len = value.length();
            nstring output;
            for (size_t i = 0; i < len; ++i)
            {
                const unsigned char c = value[i];
                output.append(lut[c >> 4]);
                output.append(lut[c & 15]);

            }
            return output;
        }


	}
}
