#pragma once
#include "public/define.h"
#include "public/decimal.h"
#include "public/object.hpp"
#include "basetype/stream.h"
#include <string>
#include <vector>

#if LIB_QT == 1
#ifdef QT4
#include "qt/qstring.h"
#else
#include "QtCore/qstring.h"
#endif // QT4


#endif

#define NSTRING_BLOCKS_SIZE 128

class NEWOBJ_API nstring_view :public stream_view
{
public:
	nstring_view();
	nstring_view(const char* value);
	nstring_view(const char* value, size_t len);
	nstring_view(const std::string& value);
	nstring_view(const stream_view& value);
	~nstring_view();

	bool operator==(const char* value) const;
	bool operator==(const std::string& value) const;
	bool operator==(const nstring_view& value) const;

	bool operator!=(const char* value) const;
	bool operator!=(const std::string& value) const;
	bool operator!=(const nstring_view& value) const;

	bool equals(const nstring_view& value) const;
	int find(char value, size_t start_idx = 0) const;
	int find(const nstring_view& value, size_t start_idx = 0) const;
	int rfind(const nstring_view& value) const;
	int rfind(char value) const;
	std::vector<int> find_list(const nstring_view& value, size_t start = 0) const;
	nstring_view substr(size_t start, size_t len) const;
	nstring_view substr(size_t start) const;
	nstring_view substr(const nstring_view& start, const nstring_view& end) const;
	nstring_view substr(size_t start, const nstring_view& end) const;
	nstring_view left(size_t len) const;
	nstring_view right(size_t len) const;
	nstring_view trim_end(char value) const;
	nstring_view trim_begin(char value) const;
	nstring_view trim(char value) const;
	operator std::string() const;
#if LIB_QT == 1
	operator QString() const;
	operator QByteArray() const;
#endif

	void print() const;
	void println() const;
	static size_t strlen(const char* value);
	static char m_empty_string[1];
};

class NEWOBJ_API nstring :public nstring_view
{
public:
	nstring();
	nstring(size_t block_size);
	nstring(const char* value);
	nstring(const char* value, size_t len);
	nstring(const std::string& value);
	nstring(const nstring& value);
	nstring(const nstring_view& value);
	nstring(const stream_view& value);
#if _HAS_CXX17 || __cplusplus >= 201703L
	nstring(const std::string_view& value);
#endif
#if LIB_QT == 1
	nstring(const QString& value);
	nstring(const QByteArray& value);
#endif
	~nstring();
	


	nstring& operator=(const char* value);
	nstring& operator=(const nstring& value);
	nstring& operator=(const std::string& value);
	nstring& operator=(const nstring_view& value);
	nstring& operator=(const stream_view& value);
#if _HAS_CXX17 || __cplusplus >= 201703L
	nstring& operator=(const std::string_view& value);
#endif
#if LIB_QT == 1
	nstring& operator=(const QString& value);
#endif
	nstring& operator+=(const nstring& value);
	nstring& operator+=(const char* value);
	//nstring& operator+=(const nstring_view& value);

	nstring operator+(const nstring& value) const;
	nstring operator+(const char* value) const;
	//nstring operator+(const nstring_view& value) const;

	bool operator<(const nstring& left) const;
	inline friend const nstring operator+(const char* a1, const nstring& a2)
	{
		return nstring(a1) += a2;
	}
	std::vector<nstring> split(const nstring_view& value)  const;
	std::vector<nstring> split(char value)  const;
	void append(const char* data, size_t length);
	void append(char value);
	void append(char value, size_t length);
	void append(const char* value);
	void append(const nstring& value);
	void append(const std::string& value);
	void append(const nstring_view& value);
	void append(const stream_view& value);
#if _HAS_CXX17 || __cplusplus >= 201703L
	void append(const std::string_view& value);
#endif
#if LIB_QT == 1
	void append(const QString& value);
#endif

	const char* c_str() const
	{
		if (m_data == nullptr || m_data_length == 0)
			return nstring::m_empty_string;
		return (char*)m_data;
	}


	
	nstring replace(size_t start, size_t len, const nstring& value) const;
	nstring remove(const nstring& value) const;



	nstring replace(const nstring& replacestring, const nstring& newstring) const;
	nstring replace(char replacestring, const nstring& newstring) const;
	nstring replace(char replacestring, char newstring) const;
	nstring replace(const nstring& replacestring, char newstring) const;


	inline bool equals(const nstring& value) const
	{
		return ::stream_view::equals((char*)value.m_data, value.m_data_length);
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

public:
	static nstring from(int32 value);
	static nstring from(uint32 value);
	static nstring from(int64 value);
	static nstring from(uint64 value);
	static nstring from(double value);
#ifndef MSVC_2010
	static nstring from(decimal value);
#endif
	static nstring from(const nstring& value);
	static nstring from(const std::string& value);
#if LIB_QT == 1
	static nstring from(const QString& value);
#endif




};
