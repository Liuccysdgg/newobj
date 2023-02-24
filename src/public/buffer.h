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
// ��󳤶�
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
#ifdef LIB_QT
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
        void append(char data);//
        void append(char data, size_t length);//
        void append(const char* data, size_t length);//
        void append(const stream_view& value);//
#ifndef MSVC_2010
        void append(std::initializer_list<uchar> char_list);
#endif
#ifdef LIB_QT
        void append(const QByteArray& data);
        operator QString() const;
        operator QByteArray() const;
#endif
        nstring to_string() const;
        nstring to_hex() const;
        static newobj::buffer from_hex(const nstring_view& hex);
    };

}


