#pragma once
#include "define.h"
#include <memory>
#include<vector>
#include "basetype/string.h"
#if LIB_QT == 1
#include "QtCore/qbytearray.h"
#endif
#include "basetype/stream.h"
namespace newobj
{
// 最大长度
// 1069894973
// 8589934592
#define NEWOBJ_BUFFER_MAX_SIZE (uint64)8589934592
    class NEWOBJ_API buffer:public stream
    {
    public:
        buffer();
        buffer(const buffer& data);
        buffer(const nstring_view& data);
        buffer(char data);
        buffer(char data, size_t length);
        buffer(const char *data, size_t length);
#if LIB_QT == 1
        buffer(const QByteArray& data);
#endif
#ifndef MSVC_2010
        buffer(std::initializer_list<uchar> char_list);
#endif
        ~buffer(); 
        newobj::buffer& operator=(const stream_view& value);
        newobj::buffer& operator=(const newobj::buffer& value);
#if LIB_QT == 1
        newobj::buffer& operator=(const QByteArray& value);
#endif
        //inline newobj::buffer operator+(const newobj::buffer& value) { return ::stream::operator+(value); }
        //stream& operator=(const newobj::buffer& value);

        static newobj::buffer from(int32 value);
        static newobj::buffer from(char value);
        static newobj::buffer from(int64 value);
        static newobj::buffer from(bool value);
        static newobj::buffer from(short value);

        operator nstring() const;
        void append(char data, size_t length);//
        void append(const char* data, size_t length);//
        void append(const stream_view& value);//
        
        void append_c(const char& value);
        void append_uc(const uchar& value);
        void append_bool(const bool& value);
        void append_us(const ushort& value,bool reverse = false);
        void append_s(const short& value,bool reverse = false);
        void append_i32(const int32& value,bool reverse = false);
        void append_ui32(const uint32& value,bool reverse = false);
        void append_i64(const int64& value,bool reverse = false);
        void append_ui64(const uint64& value,bool reverse = false);
        void append_double(const double& value,bool reverse = false);
        void append_float(const float& value,bool reverse = false);
#ifndef MSVC_2010
        void append(std::initializer_list<uchar> char_list);
#endif
#if LIB_QT == 1
        void append(const QByteArray& data);
        operator QString() const;
        operator QByteArray() const;
#endif
        nstring to_string() const;
        nstring to_hex() const;
        static newobj::buffer from_hex(const nstring_view& hex);
    };

}


