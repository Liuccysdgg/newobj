#include "buffer.h"
#include "exception.h"
#include <string.h>
#include <sstream>
#include "util/bytes.h"
#include "util/mem.h"
namespace newobj
{
    buffer::buffer():stream()
    {
        m_tail_blank = false;
    }

    buffer::buffer(const buffer& data):stream(data.data(),data.length())
    {
        m_tail_blank = false;
        append(data);
    }

    buffer::buffer(const char* data, size_t len):stream(data,len)
    {
        m_tail_blank = false;
    }

    buffer::buffer(const nstring_view& data):stream(data.data(),data.length())
    {
        m_tail_blank = false;
    }

    buffer::buffer(char data):stream(&data,1)
    {
        m_tail_blank = false;
    }
    buffer::buffer(char data, size_t length)
    {
        ::stream::append(data, length);
    }
#ifdef LIB_QT
    buffer::buffer(const QByteArray& data): stream()
    {
        m_tail_blank = false;
        ::stream::append(data.data(),data.length());
    }
#endif
#ifndef MSVC_2010
    buffer::buffer(std::initializer_list<uchar> char_list) : stream()
    {
        m_tail_blank = false;
        ::buffer::append(char_list);
    }
#endif
    buffer::~buffer()
    {
        clear();
    }
    newobj::buffer& buffer::operator=(const stream_view& value)
    {
        clear();
        ::stream::append(value.data(), value.length());
        return *this;
    }
    newobj::buffer& buffer::operator=(const newobj::buffer& value)
    {
        clear();
        ::stream::append(value.data(), value.length());
        return *this;
    }
    newobj::buffer& buffer::operator=(const QByteArray& value)
    {
        clear();
        ::stream::append(value.data(), value.length());
        return *this;
    }
    newobj::buffer buffer::operator+(const newobj::buffer& value)
    {
        	if (value.length() == 0)
                return *this;
	        stream result;
	        result.append(*this);
	        result.append(value);
	        return result;
    }
    newobj::buffer& buffer::operator+=(const newobj::buffer& value)
    {
        if (value.length() == 0)
            return *this;
        append(value);
        return *this;
    }
    newobj::buffer buffer::from(int32 value)
    {
        return bytes::to_buffer((int32)value);
    }
    newobj::buffer buffer::from(char value)
    {
        return bytes::to_buffer((uchar)value);
    }
    newobj::buffer buffer::from(int64 value)
    {
        return bytes::to_buffer((int64)value);
    }
    newobj::buffer buffer::from(bool value)
    {
        return bytes::to_buffer((uchar)value);
    }
    newobj::buffer buffer::from(short value)
    {
        return bytes::to_buffer((short)value);
    }
    buffer::operator nstring() const
    {
        return to_string();
    }
#ifndef MSVC_2010
    void buffer::append(std::initializer_list<uchar> char_list)
    {
        if (char_list.size() == 0)
            return;
        char* new_buf = (char*)mem::malloc(char_list.size());
        uint64 idx = 0;
        for (auto c : char_list)
        {
            new_buf[idx] = c;
            idx++;
        }
        ::stream::append(new_buf, idx);
        mem::free(new_buf);
        
    }
#endif
#ifdef LIB_QT
    void buffer::append(const QByteArray& data)
    {
        ::stream::append(data.data(), data.length());
    }
    buffer::operator QString() const
    {
        return this->to_string();
    }
    buffer::operator QByteArray() const
    {
        QByteArray result;
        result.append(this->data(),this->length());
        return result;
    }
#endif
   
    nstring buffer::to_string() const
    {
        nstring ret;
        if (length() == 0)
            return "";
        size_t over_idx = 0;
        for (size_t i = 0; i < length(); i++)
        {
            if (m_data[i] == 0)
            {
                over_idx = i;
                break;
            }
        }
        if (over_idx == 0)
            over_idx = length(); 
        ret.append((const char*)data(), over_idx);
        return ret;
    }

    nstring buffer::to_hex() const
    {
        unsigned char highByte, lowByte;
        size_t length = ::stream::length();
        nstring dest("0", length*2);
        for (size_t i = 0; i < length; i++)
        {
            highByte = (*this)[i] >> 4;
            lowByte = (*this)[i] & 0x0f;

            highByte += 0x30;

            if (highByte > 0x39)
                dest[i * 2] = highByte + 0x07;
            else
                dest[i * 2] = highByte;

            lowByte += 0x30;
            if (lowByte > 0x39)
                dest[i * 2 + 1] = lowByte + 0x07;
            else
                dest[i * 2 + 1] = lowByte;
        }
        return dest;
    }

    newobj::buffer buffer::from_hex(const nstring_view& hex)
    {
        newobj::buffer result;
        auto len = hex.length();
        for (decltype(len) i = 0; i < len; i += 2)
        {
            unsigned int element;
            std::istringstream strHex(hex.substr(i, 2));
            strHex >> std::hex >> element;
            ((stream*)&result)->append(static_cast<char>(element));
        }
        return result;
    }
}
