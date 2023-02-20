#pragma once
#include "public/define.h"
#include "public/decimal.h"
#include "public/object.hpp"
#include <string>
#include <vector>
#include "util/mem.h"
#include "string.h"
#include <cstring>
#ifdef LIB_QT
#ifdef QT4
#include "qt/qstring.h"
#else
#include "QtCore/qstring.h"
#endif // QT4


#endif

// NSTRING 块分配大小
#define NSTRING_BLOCKS_SIZE 32


/*基础C语言字符串封装结构体*/
struct ncstr{
    ncstr(){
        m_value = nullptr;
        m_len = 0;
    }
    ncstr(const char* value){
        this->operator=(value);
    }
    ~ncstr(){
        clear();
    }
    void clear(){
        if(m_value != nullptr)
            mem::free(m_value);
        m_value = nullptr;
        m_len = 0;
    }
    void operator=(const char* value){
        size_t src_len = strlen(value);
        if(src_len == 0 || value == nullptr)
            return;
        if(src_len+1>m_len){
            clear();
            m_value = (char*)mem::malloc(src_len+1);
        }
        memcpy(m_value,value,src_len);
        m_value[src_len] = 0;
        m_len = src_len;
    }
    char* m_value;
    size_t m_len;
};


#define INIT_NSTRING_VIEW(VIEW) VIEW,nullptr,1


class NEWOBJ_API nstring_view:public object
{
public:
    nstring_view(bool view,void* nm1,int nm2);
    nstring_view(const char* value,size_t length);
    nstring_view(const char* value);
    nstring_view();
	~nstring_view();
	bool operator==(const char* value) const;
	bool operator==(const nstring_view& value) const;
	bool operator==(const std::string& value) const;
	bool operator!=(const char* value) const;
	bool operator!=(const nstring_view& value) const;
	bool operator!=(const std::string& value) const;

	bool operator<(const nstring_view& left) const;

	inline size_t length() const { return m_data_length; }
	inline bool empty() const { return m_data == nullptr || m_data_length == 0; }
	char& operator[](size_t index) const;
	char at(size_t index) const;
	bool equals(const char* value,size_t size) const;
	operator std::string() const;
#ifdef LIB_QT
	operator QString() const;
	operator QByteArray() const;
#endif

	void print() const;
	void println() const;

	const char* c_str() const
	{
        if (m_data == nullptr || m_data_length == 0)
            return nstring_view::m_empty_string;
        return m_data;
	}

	int32 to_int32() const;
	uint32 to_uint32() const;
	int64 to_int64() const;
	uint64 to_uint64() const;
	short to_short() const;
	ushort to_ushort() const;
#ifndef MSVC_2010
	decimal to_decimal() const;
#endif
	double to_double() const;
	float to_float() const;

	bool is_num() const;
	bool is_decimal() const;

	size_t find(char value, size_t start_idx = 0) const;
	size_t find(const nstring_view& value, size_t start_idx = 0) const;
	size_t rfind(const nstring_view& value) const;
	size_t rfind(char value) const;
	std::vector<size_t> find_list(const nstring_view& value, size_t start = 0) const;
	nstring_view substr(size_t start, size_t len) const;
	nstring_view substr(size_t start) const;
	nstring_view substr(const nstring_view& start, const nstring_view& end) const;
	nstring_view substr(size_t start, const nstring_view& end) const;

	nstring_view left(size_t len) const;
	nstring_view right(size_t len) const;
	std::vector<nstring_view> split_view(const nstring_view& value)  const;
	std::vector<nstring_view> split_view(char value)  const;

	nstring_view trim_end(char value) const;
	nstring_view trim_begin(char value) const;
	nstring_view trim(char value) const;


	

	inline bool equals(const nstring_view& value) const
	{
		return equals(value.m_data,value.m_data_length);
	}
public:
	static size_t strlen(const char* value);

protected:
	int compare(const nstring_view& right) const;
	bool lenlegal(size_t len, bool throw_exp = true) const;
protected:
	static char m_empty_string[1];
	char* m_data;
	size_t m_data_length;
	size_t m_mem_length;
	size_t m_block_size;
    bool m_view;
};



//#define NSTRING_FILE
class NEWOBJ_API nstring:public nstring_view
{
public:
	nstring();
	nstring(size_t block_size);
	nstring(const char* value);
	nstring(const char* value, size_t len);
	nstring(const std::string& value);
	nstring(const nstring& value);
#if _HAS_CXX17 || __cplusplus >= 201703L
	nstring(const std::string_view& value);
#endif
#ifdef LIB_QT
	nstring(const QString& value);
	nstring(const QByteArray& value);
#endif
	~nstring();
	nstring(const nstring_view& view);

    nstring& operator=(const nstring_view& view);
	nstring& operator=(const char* value);
	nstring& operator=(const nstring& value);
	nstring& operator=(const std::string& value);
#if _HAS_CXX17 || __cplusplus >= 201703L
	nstring& operator=(const std::string_view& value);
#endif
#ifdef LIB_QT
	nstring& operator=(const QString& value);
#endif
	nstring& operator+=(const nstring& value);
	nstring& operator+=(const char* value);

	nstring operator+(const nstring& value) const;
	nstring operator+(const char* value) const;

	inline friend const nstring operator+(const char* a1, const nstring& a2) 
	{
		return nstring(a1) += a2;
	}


    void append(const nstring_view& view);
	void append(const char* data, size_t length);
	void append(char value);
	void append(char value,size_t length);
	void append(const char* value);
	void append(const nstring& value);
	void append(const std::string& value);
#if _HAS_CXX17 || __cplusplus >= 201703L
	void append(const std::string_view& value);
#endif
#ifdef LIB_QT
	void append(const QString& value);
#endif
	inline void clear() { m_data_length = 0; }
	nstring replace(size_t start, size_t len, const nstring_view& value) const;
	nstring remove(const nstring_view& value) const;


    std::vector<nstring> split(const nstring_view& value)  const;
	std::vector<nstring> split(char value)  const;


    nstring replace(const nstring_view& replacestring, const nstring_view& newstring) const;
	nstring replace(char replacestring, const nstring_view& newstring) const;
	nstring replace(char replacestring, char newstring) const;
	nstring replace(const nstring_view& replacestring, char newstring) const;
	static nstring from(int32 value);
	static nstring from(uint32 value);
	static nstring from(int64 value);
	static nstring from(uint64 value);
	static nstring from(double value);
#ifndef MSVC_2010
	static nstring from(decimal value);
#endif
	static nstring from(const nstring_view& value);
	static nstring from(const std::string& value);
#ifdef LIB_QT
	static nstring from(const QString& value);
#endif


};
