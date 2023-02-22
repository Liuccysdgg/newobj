#pragma once
#include "define.h"
#include <memory>
#include<vector>
#include "basetype/string.h"
#ifdef LIB_QT
#include "QtCore/qbytearray.h"
#endif
namespace newobj
{
// 最大长度
// 1069894973
// 8589934592
#define NEWOBJ_BUFFER_MAX_SIZE (uint64)8589934592
    class NEWOBJ_API buffer
    {
    public:
        buffer();
        buffer(const buffer& data);
        buffer(const uchar* data, size_t len);
        buffer(const nstring& data); 
        buffer(char data);
        buffer(size_t length, char data);
#ifdef LIB_QT
        buffer(const QByteArray& data);
#endif
#ifndef MSVC_2010
        buffer(std::initializer_list<uchar> char_list);
#endif
        ~buffer(); 


        bool deepcopy();
        /*深拷贝*/
        void deepcopy(bool deep);

        size_t length() const;
        const char* data() const;
        void append(const buffer& value);
        void append(const char* data, size_t len);
        void append(const uchar* data, size_t len);
        void append(const nstring& data);
        void append(char data);
        
        static newobj::buffer from(int32 value);
        static newobj::buffer from(char value);
        static newobj::buffer from(int64 value);
        static newobj::buffer from(bool value);
        static newobj::buffer from(short value);

        operator nstring() const;
        bool operator==(const newobj::buffer& data) const;
#ifndef MSVC_2010
        void append(std::initializer_list<uchar> char_list);
#endif
#ifdef LIB_QT
        void append(const QByteArray& data);
        operator QString() const;
        operator QByteArray() const;
#endif
        uchar at(size_t idx) const;
        buffer right(size_t len) const;
        buffer left(size_t len) const;
        void clear();
        size_t size() const;
        void setsize(size_t len, bool setZero = true);
        void set(size_t start, const buffer& data);
        buffer replace(size_t start, size_t len, const buffer& data);
        uchar& operator[](size_t idx) const;
        buffer operator+(const buffer& p1) const;
        void operator=(const buffer& data);
        size_t find(size_t start,const buffer& buffer) const;
        size_t find(size_t start, const char* find_buf, size_t find_len) const;
        bool remove(size_t start, size_t len);
        newobj::buffer sub(size_t start, size_t len) const;
        std::vector<newobj::buffer> split(const buffer& cut) const;
        std::vector<size_t> cut(const buffer& cut) const;
        nstring to_string() const;
        nstring to_hex() const;
        static newobj::buffer from_hex(const nstring& hex);

        newobj::buffer trim_end(char value) const;
        newobj::buffer trim_begin(char value) const;
        newobj::buffer trim(char value) const;

        void __set(uchar* data,size_t length);
    private:
        size_t m_len;
        uchar* m_data;
        // TRUE=深拷贝 FALSE=浅拷贝
        bool m_deepcopy;
    };

}


