#include "bytes.h"
#include <sstream>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#endif
namespace newobj
{
    namespace bytes
    {
            bool search(const char* src_buf, uint32 src_len, const char* search_buf, uint32 search_len)
            {
                int index = 0;
                for (size_t i = 0; i < src_len; i++)
                {
                    for (size_t x = 0; x < search_len; x++)
                    {
                        if (i + index > src_len - 1)
                            break;
                        if (search_buf[x] == src_buf[i + index])
                        {
                            index++;
                            if (x + 1 == search_len)
                                return true;
                        }
                        else
                            break;
                    }
                }
                return false;
            }
#define _GET_BIT(num,pos) return num & (1 << pos)?1:0;
            uchar bit32(uint32 num, int pos)
            {
                _GET_BIT(num, pos);
            }
            uchar bit16(ushort num, int pos)
            {
                _GET_BIT(num, pos);
            }
            uchar bit8(uchar num, int pos)
            {
                _GET_BIT(num, pos);
            }


#define _BIT_SET(v,num,bit) int a=1;a<<=num;if(bit == 1){v|=a;}else{v^=a;}
            /*置位*/
            void bit64(uint64 &num, int pos, uchar bit)
            {
                _BIT_SET(num, pos, bit);
            }
            void bit32(uint32 &num, int pos, uchar bit)
            {
                _BIT_SET(num, pos, bit);
            }
            void bit16(ushort &num, int pos, uchar bit)
            {
                _BIT_SET(num, pos, bit);
            }
            void bit8(uchar &num, int pos, uchar bit)
            {
                _BIT_SET(num, pos, bit);
            }



            nstring to_hexstring(const ushort hex)
            {
                nstring ret;
                uchar new_buf_ptr[20];
                for (uint32 i = 0; i < 2; i++)
                {
                    memset(new_buf_ptr, 0, 20);
#ifdef _WIN32
                    SPRINTF((char*)new_buf_ptr,20, "%1X", (char)*(&hex + i));
#else
                    SPRINTF((char*)new_buf_ptr, "%1X", (char)*(&hex + i));
#endif
                    if (new_buf_ptr[0] == '\0')
                    {
                        abort();
                    }
                    if (strlen((char*)new_buf_ptr) > 4)
                    {
                        new_buf_ptr[0] = new_buf_ptr[6];
                        new_buf_ptr[1] = new_buf_ptr[7];
                        new_buf_ptr[2] = '\0';
                    }
                    //newobj::print::str(new_buf_ptr, CONSOLE_GREEN, CONSOLE_BG_BLUE, 0);
                    ret.append((char*)new_buf_ptr);
                }
                return ret;
            }

            nstring to_hexstring(const int32 dec)
            {
                nstring ret;
                char buf[20];
                memset(buf, 0, 20);
#ifdef _WIN32
                SPRINTF(buf,20, "%05x", dec);
#else
                SPRINTF(buf, "%05x", dec);
#endif
                if (strlen(buf) == 8)
                    return buf;
                for (size_t i = 0; i < 8 - strlen(buf)+1; i++)
                    ret += "0";
                ret += buf;
                return ret;
            }

            uint32 hex_to_uint32(const nstring& src)
            {
                std::stringstream ss2;
                uint32 d2;
                ss2 << std::hex << src.c_str(); //选用十六进制输出
                ss2 >> d2;
                return d2;
            }

            uint64 hex_to_uint64(const nstring& src)
            {
                std::stringstream ss2;
                uint64 d2;
                ss2 << std::hex << src.c_str(); //选用十六进制输出
                ss2 >> d2;
                return d2;
            }


            void to_int(int32& dest, const char* src, bool reverse)
            {
                char* dest_char = (char*)&dest;
                for (int i = 0; i < sizeof(dest); i++)
                    dest_char[i] = src[(reverse == true ? sizeof(dest) - 1 - i : i)];
                //dest = atoi(src);
            }
            void to_uint(uint32& dest, const char* src, bool reverse)
            {
                char* dest_char = (char*)&dest;
                for (int i = 0; i < sizeof(dest); i++)
                    dest_char[i] = src[(reverse == true ? sizeof(dest) - 1 - i : i)];

                // dest = atoi(src);
            }

            void to_short(short& dest, const char* src, bool reverse)
            {
                char* dest_char = (char*)&dest;
                for (int i = 0; i < sizeof(dest); i++)
                    dest_char[i] = src[(reverse == true ? sizeof(dest) - 1 - i : i)];
            }

            void to_ushort(ushort& dest, const char* src, bool reverse)
            {
                char* dest_char = (char*)&dest;
                for (int i = 0; i < sizeof(dest); i++)
                    dest_char[i] = src[(reverse == true ? sizeof(dest) - 1 - i : i)];
            }

            void to_char(char* dest, short src, bool reverse)
            {
                char* src_char = (char*)&src;
                for (int i = 0; i < sizeof(src); i++)
                    dest[i] = src_char[(reverse == true ? sizeof(src) - 1 - i : i)];
            }
            void to_char(char* dest, int32 src, bool reverse)
            {
                char* src_char = (char*)&src;
                for (int i = 0; i < sizeof(src); i++)
                    dest[i] = src_char[(reverse == true ? sizeof(src) - 1 - i : i)];
            }
            void to_char(char* dest, int64 src, bool reverse)
            {
                char* src_char = (char*)&src;
                for (int i = 0; i < sizeof(src); i++)
                    dest[i] = src_char[(reverse == true ? sizeof(src) - 1 - i : i)];
            }

            newobj::buffer  to_buffer(short src, bool reverse)
            {
                newobj::buffer result;
                result.setsize(2);
                to_char((char*)result.data(),(short)src,reverse);
                return result;
            }

            newobj::buffer  to_buffer(int32 src, bool reverse)
            {
                newobj::buffer result;
                result.setsize(4);
                to_char((char*)result.data(), (int32)src, reverse);
                return result;
            }

            newobj::buffer  to_buffer(int64 src, bool reverse)
            {
                newobj::buffer result;
                result.setsize(8);
                to_char((char*)result.data(), (int64)src, reverse);
                return result;
            }

            newobj::buffer  to_buffer(uchar src)
            {
                newobj::buffer result;
                result.setsize(1);
                result[0] = src;
                return result;
            }



            void to_uint64(uint64& dest, const char* src, bool reverse)
            {
                char* dest_char = (char*)&dest;
                for (int i = 0; i < sizeof(dest); i++)
                    dest_char[i] = src[(reverse == true ? sizeof(dest) - 1 - i : i)];
            }

            void to_int64(int64& dest, const char* src, bool reverse)
            {
                char* dest_char = (char*)&dest;
                for (int i = 0; i < sizeof(dest); i++)
                    dest_char[i] = src[(reverse == true ? sizeof(dest) - 1 - i : i)];
            }

            nstring  to_string(const char* src, bool reverse)
            {
                nstring result;
                int i = 0;
                while (true) {
                    if (src[i] == 0)
                        return result;
                    result.append((char*)&src[(reverse == true ? sizeof(result) - 1 - i : i)],1);
                    i++;
                }
                return result;
                    
            }
#ifdef _WIN32
            newobj::buffer to_unicode(const newobj::buffer& data)
            {
                int len = 0;
                len = MultiByteToWideChar(CP_ACP, 0, data.data(), (int)data.length(), 0, 0);
                if (len <= 0)
                    return newobj::buffer();
                wchar_t* buf = new wchar_t[len];
                MultiByteToWideChar(CP_ACP, 0, data.data(), (int)data.length(), buf, len);
                newobj::buffer result;
                result.append((uchar*)buf, len * 2);
                delete[] buf;
                
                return result;
            }
#endif
           
        }
}
