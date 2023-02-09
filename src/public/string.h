#pragma once
#include "public/define.h"
#include "public/decimal.h"
#include <string>
#include <vector>

#ifdef LIB_QT
#ifdef QT4
#include "qt/qstring.h"
#else
#include "QtCore/qstring.h"
#endif // QT4


#endif

#define NSTRING_BLOCKS_SIZE 32
//#define NSTRING_FILE
#ifdef NSTRING_FILE
namespace newobj
{
	class file;
}
#endif 
class NEWOBJ_API nstring
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
#ifdef NSTRING_FILE
	bool open(const nstring& filepath);
#endif
	bool operator==(const char* value) const;
	bool operator==(const nstring& value) const;
	bool operator==(const std::string& value) const;


	bool operator!=(const char* value) const;
	bool operator!=(const nstring& value) const;
	bool operator!=(const std::string& value) const;

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
	bool operator<(const nstring& left) const;
	inline friend const nstring operator+(const char* a1, const nstring& a2) 
	{
		return nstring(a1) += a2;
	}


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
            return nstring::m_empty_string;
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
	size_t find(const nstring& value, size_t start_idx = 0) const;
	size_t rfind(const nstring& value) const;
	size_t rfind(char value) const;
	std::vector<size_t> find_list(const nstring& value, size_t start = 0) const;
	nstring substr(size_t start, size_t len) const;
	nstring substr(size_t start) const;
	nstring substr(const nstring& start, const nstring& end) const;
	nstring substr(size_t start, const nstring& end) const;

	nstring left(size_t len) const;
	nstring right(size_t len) const;
	nstring replace(size_t start, size_t len, const nstring& value) const;
	nstring remove(const nstring& value) const;
	std::vector<nstring> split(const nstring& value)  const;
	std::vector<nstring> split(char value)  const;
	nstring trim_end(char value) const;
	nstring trim_begin(char value) const;
	nstring trim(char value) const;


	nstring replace(const nstring& replacestring, const nstring& newstring) const;
	nstring replace(char replacestring, const nstring& newstring) const;
	nstring replace(char replacestring, char newstring) const;
	nstring replace(const nstring& replacestring, char newstring) const;


	inline bool equals(const nstring& value) const
	{
		return equals(value.m_data,value.m_data_length);
	}
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
#ifdef LIB_QT
	static nstring from(const QString& value);
#endif
	static size_t strlen(const char* value);

private:
	int compare(const nstring& right) const;
	bool lenlegal(size_t len, bool throw_exp = true) const;
private:
	static char m_empty_string[1];
	char* m_data;
	size_t m_data_length;
	size_t m_mem_length;
	size_t m_block_size;

#ifdef NSTRING_FILE
	newobj::file* m_file;
#endif
};
