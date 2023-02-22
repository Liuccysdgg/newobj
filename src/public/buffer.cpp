#include "buffer.h"
#include "exception.h"
#include <string.h>
#include <sstream>
#include "util/bytes.h"
#include "util/mem.h"
namespace newobj
{
#define CLEAR this->m_data = nullptr;this->m_len = 0;m_deepcopy = true;

    buffer::buffer()
    {
        CLEAR;
    }

    buffer::buffer(const buffer& data)
    {
        CLEAR;
        append(data);
    }

    buffer::buffer(const uchar* data, size_t len)
    {
        CLEAR;
        append(data, len);
    }

    buffer::buffer(const nstring& data)
    {
        CLEAR;
        append(data);
    }

    buffer::buffer(char data)
    {
        CLEAR;
        append(data);
    }
    buffer::buffer(size_t length, char data)
    {
        CLEAR;
        setsize(length,false);
        memset(m_data,data,length);
    }
#ifdef LIB_QT
    buffer::buffer(const QByteArray& data)
    {
        CLEAR;
        append(data);
    }
#endif
#ifndef MSVC_2010
    buffer::buffer(std::initializer_list<uchar> char_list)
    {
		CLEAR;
        append(char_list);
    }
#endif
    buffer::~buffer()
    {
        clear();
    }

    bool buffer::deepcopy()
    {
        return m_deepcopy;
    }

    void buffer::deepcopy(bool deep)
    {
        m_deepcopy = deep;
    }

    size_t buffer::length() const
    {
        return this->m_len;
    }

    const char* buffer::data() const
    {
        return (const char*)m_data;
    }

    void buffer::append(const buffer& value)
    {
        if (value.m_len == 0)
            return;
        append(value.m_data, value.m_len);
    }

    void buffer::append(const char* data, size_t len)
    {
        append((const uchar*)data,len);
    }

    void buffer::append(const uchar* data, size_t len)
    {
        if (data == nullptr || len <= 0)
            return;
        size_t new_len = len + m_len;
        if (new_len >= NEWOBJ_BUFFER_MAX_SIZE)
        {
            trw_str("Maximum length exceeded");
        }
        uchar* new_data = (uchar*)mem::realloc(m_data,new_len);
        if(new_data == nullptr){
            new_data = (uchar*)mem::malloc(new_len);
            memcpy(new_data, m_data, m_len);
            memcpy(new_data + m_len, data, len);
            clear();
        }else{
            if(this->m_data == nullptr){
                memcpy(new_data, data, len);
            }else{
                memcpy(new_data + m_len, data, len);
            }

        }
        this->m_data = (uchar*)new_data;
        this->m_len = new_len;
    }

    void buffer::append(const nstring& data)
    {
        append(data.c_str(), data.length());
    }

    void buffer::append(char data)
    {
        append(&data, 1);
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
    bool buffer::operator==(const newobj::buffer& data) const
    {
        if (data.length() != this->length())
            return false;
        for (size_t i = 0; i < this->size(); i++)
        {
            if (data[i] != (*this)[i])
                return false;
        }
        return true;
    }
#ifndef MSVC_2010
    void buffer::append(std::initializer_list<uchar> char_list)
    {
        if (char_list.size() == 0)
            return;


        uchar* new_buf = (uchar*)mem::malloc(char_list.size());
        uint64 idx = 0;
        for (auto c : char_list)
        {
            new_buf[idx] = c;
            idx++;
        }
        this->append(new_buf,char_list.size());
        mem::free(new_buf);
    }
#endif
#ifdef LIB_QT
    void buffer::append(const QByteArray& data)
    {
        this->append(data.data(), data.length());
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
    uchar buffer::at(size_t idx) const
    {
        return this->operator[](idx);
    }
    buffer buffer::right(size_t len) const
    {
        buffer buf;
        if (len > m_len)
            return *this;
        buf.append(m_data + length() - len, len);
        return buf;
    }

    buffer buffer::left(size_t len) const
    {
        buffer buf;
        if (len > m_len)
            return *this;
        buf.append(m_data, len);
        return buf;
    }

    void buffer::clear()
    {
        if (m_deepcopy)
        {
            if (m_data != nullptr)
                mem::free(m_data);
        }
        //this->m_deepcopy = true;
        this->m_len = 0;
        this->m_data = nullptr;
    }

    void buffer::setsize(size_t len, bool setZero)
    {
        clear();
        if (len >= NEWOBJ_BUFFER_MAX_SIZE)
        {
            trw_str("Maximum length exceeded");
        }

        this->m_len = len;
        this->m_data = (uchar*)mem::malloc(this->m_len);
        memset(this->m_data, 0, this->m_len);
    }

    void buffer::set(size_t start, const buffer& data)
    {
        if (data.length() + start > length())
            return;
        memcpy((char*)m_data + start, data.m_data, (size_t)data.m_len);
    }

    buffer buffer::replace(size_t start, size_t len, const buffer& data)
    {
        buffer ret;
        buffer left_buf;
        buffer right_buf;

        left_buf = left(start);
        right_buf = right(length() - start - len);

        ret.append(left_buf);
        ret.append(data);
        ret.append(right_buf);
        return ret;
    }

    size_t buffer::size() const
    {
        return m_len;
    }

    uchar& buffer::operator[](size_t idx) const
    {
        if (idx + 1 > m_len)
        {
            trw_str("Indice troppo lungo");
        }
        return m_data[idx];
    }

    buffer buffer::operator+(const buffer& p1) const
    {
        buffer b = *this;
        b.append(p1);
        return b;
    }

    void buffer::operator=(const buffer& data)
    {
        clear();
        append(data);
    }

    size_t buffer::find(size_t start, const buffer& buffer) const
    {
        return find(start, (const char*)buffer.data(),buffer.length());
    }

    size_t buffer::find(size_t start, const char* find_buf, size_t find_len) const
    {
        if (find_len == 0 || start + find_len > this->length())
            return -1;
        for (size_t i = start; i < length(); i++)
        {
            if (length() - i < find_len)
                return -1;
            bool isequals = true;
            for (size_t x = 0; x < find_len; x++)
            {
                uchar m_data_char = m_data[i + x];
                uchar find_buf_char = find_buf[x];
                if (m_data_char != find_buf_char)
                {
                    isequals = false;
                    break;
                }
            }
            if (isequals)
                return i;
        }
        return -1;
    }

    bool buffer::remove(size_t start, size_t len)
    {
        // 0 [1] [2] 3 4 5
        if (start + len > length())
            return false;

        buffer left_buf = left(start);
        buffer right_buf = right(length() - start - len);
        this->clear();
        this->append(left_buf);
        this->append(right_buf);
        return true;
    }

    newobj::buffer buffer::sub(size_t start, size_t len) const
    {
        newobj::buffer result;
        if (start + len > length())
        {
            return sub(start, length() - start);
        }
        result.append(m_data+start,len);
        return result;
    }

    std::vector<newobj::buffer> buffer::split(const buffer& cut) const
    {
        std::vector<newobj::buffer> ret_vct;
        size_t start = 0;
        while (true)
        {
            auto idx = this->find(start,cut);
            if (idx == -1)
            {
                if (start  < length())
                    ret_vct.push_back(this->right(length() - start));
                return ret_vct;
            }
            newobj::buffer node;
            ret_vct.push_back(this->sub(start, idx - start));
            start = idx+cut.length();
        }
        return ret_vct;
    }
    std::vector<size_t> buffer::cut(const buffer& cut) const
    {
        std::vector<size_t> ret_vct;
        uint32 start = 0;
        while (true)
        {
            auto idx = this->find(start, cut);
            if (idx == -1)
                return ret_vct;
            start = idx + cut.length();
            ret_vct.push_back(idx);
        }
        return ret_vct;
    }

    nstring buffer::to_string() const
    {
        nstring ret;
        if (length() == 0)
            return "";
        for (size_t i = 0; i < m_len; i++)
        {
            if (m_data[i] == 0)
            {
                ret.append((const char*)m_data, i);
                return ret;
            }
        }
        ret.append((const char*)m_data,m_len);
        return ret;
    }

    nstring buffer::to_hex() const
    {
        unsigned char highByte, lowByte;
        size_t length = size();
        nstring dest("0",size()*2);
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

    newobj::buffer buffer::from_hex(const nstring& hex)
    {
        newobj::buffer result;
        auto len = hex.length();
        for (decltype(len) i = 0; i < len; i += 2)
        {
            unsigned int element;
            std::istringstream strHex(hex.substr(i, 2));
            strHex >> std::hex >> element;
            result.append(static_cast<char>(element));
        }
        return result;
    }
    newobj::buffer buffer::trim_begin(char value) const
    {
        size_t idx = 0;
        for (size_t i = 0; i < length(); i++)
        {
            if (operator[](i) == value)
                idx++;
            else
                break;
        }
        return this->right(length() - idx);
    }

    newobj::buffer buffer::trim(char value) const
    {
        return trim_begin(value).trim_end(value);
    }

    newobj::buffer buffer::trim_end(char value) const
    {
        size_t idx = 0;
        for (size_t i = 0; i < length(); i++)
        {
            if (operator[](length() - i - 1) == value)
                idx++;
            else
                break;
        }
        return this->left(length() - idx);
    }

    void buffer::__set(uchar *data, size_t length)
    {
        clear();
        m_data = data;
        m_len = length;

    }

}
