#include "string.h"
#include "public/exception.h"
#include "util/mem.h"
#include "util/file.h"
#include <string.h>
#include <iostream>
char nstring_view::m_empty_string[1] = { '\0' };
#define INIT_STRING	m_tail_blank = true;m_block_size = NSTRING_BLOCKS_SIZE;m_data_length = 0;m_mem_length = m_block_size;m_data = (char*)mem::malloc(m_block_size);memset(m_data,0,m_block_size)
nstring_view::nstring_view():stream_view()
{
	
}
nstring_view::nstring_view(const std::string& value) :stream_view(value.c_str(), value.length())
{

}
nstring_view::nstring_view(const char* value):stream_view(value,strlen(value))
{

}
nstring_view::nstring_view(const char* value, size_t len):stream_view(value,len)
{

}
nstring_view::nstring_view(const stream_view& value):stream_view(value)
{

}
nstring_view::~nstring_view()
{

}

nstring_view::operator std::string() const
{
	return std::string(data(), length());
}
#ifdef LIB_QT
nstring_view::operator QString() const
{
	return this->operator QByteArray();
}
nstring_view::operator QByteArray() const
{
	return QByteArray(data(),length());
}
#endif

bool nstring_view::operator==(const char* value)  const
{
	return ::stream_view::equals(value, ::nstring_view::strlen(value));
}
bool nstring_view::operator==(const std::string& value) const
{
	return ::stream_view::equals(value.data(), value.length());
}
bool nstring_view::operator==(const nstring_view& value) const
{
	return ::stream_view::equals(value.data(), value.length());
}
bool nstring_view::operator!=(const char* value) const
{
	return !::stream_view::equals(value, nstring::strlen(value));
}

bool nstring_view::operator!=(const nstring_view& value) const
{
	return !::stream_view::equals(value.data(), value.length());
}

bool nstring_view::operator!=(const std::string& value) const
{
	return !::stream_view::equals(value.c_str(), value.length());
}


bool nstring_view::equals(const nstring_view& value) const
{
	return ::stream_view::equals(value.data(), value.length());
}

size_t nstring_view::find(char value, size_t start_idx) const
{
	return ::stream_view::find(value,start_idx);
}

size_t nstring_view::find(const nstring_view& value, size_t start_idx) const
{
	return ::stream_view::find(value, start_idx);
}

size_t nstring_view::rfind(const nstring_view& value) const
{
	return ::stream_view::rfind(value);
}

size_t nstring_view::rfind(char value) const
{
	return ::stream_view::rfind(value);
}

std::vector<size_t> nstring_view::find_list(const nstring_view& value, size_t start) const
{
	return ::stream_view::find_list(value,start);
}

nstring_view nstring_view::substr(size_t start, size_t len) const
{
	return ::stream_view::substr(start, len);
}

nstring_view nstring_view::substr(size_t start) const
{
	return ::stream_view::substr(start);
}

nstring_view nstring_view::substr(const nstring_view& start, const nstring_view& end) const
{
	return ::stream_view::substr(start,end);
}

nstring_view nstring_view::substr(size_t start, const nstring_view& end) const
{
	return ::stream_view::substr(start, end);
}

nstring_view nstring_view::left(size_t len) const
{
	return ::stream_view::left(len);
}

nstring_view nstring_view::right(size_t len) const
{
	return ::stream_view::right(len);
}
nstring_view nstring_view::trim_end(char value) const
{
	return ::stream_view::trim_end(value);
}

nstring_view nstring_view::trim_begin(char value) const
{
	return ::stream_view::trim_begin(value);
}

nstring_view nstring_view::trim(char value) const
{
	return ::stream_view::trim(value);
}





void nstring_view::print() const
{
	t_ret(m_data == nullptr);
	std::cout << std::string(m_data, m_data_length);
}
void nstring_view::println() const
{
	t_ret(m_data == nullptr);
	std::cout << std::string(m_data, m_data_length) << std::endl;
}

int32 nstring::to_int32() const
{
	if (is_num())
	{
		return atoi(std::string(data(), length()).c_str());
	}
	else if (is_decimal())
	{
		return (int32)to_double();
	}
	return 0;
}
uint32 nstring::to_uint32() const
{
	return (uint32)to_int32();
}
int64 nstring::to_int64() const
{
	if (is_num())
	{
#ifdef _WIN32
		return _atoi64(std::string(data(), length()).c_str());
#else
		return atoll(std::string(data(), length()).c_str());
#endif
	}
	else if (is_decimal())
	{
		return (int64)to_double();
	}
	return 0;
}
uint64 nstring::to_uint64() const
{
	return (uint64)to_int64();
}

short nstring::to_short() const
{
	return (short)to_int32();
}
ushort nstring::to_ushort() const
{
	return (ushort)to_short();
}
#ifndef MSVC_2010
decimal nstring::to_decimal() const
{
	f_ret_var(is_decimal(), 0.0);
	return decimal(atof(std::string(data(), length()).c_str()));
}
#endif
double nstring::to_double() const
{
	f_ret_var(is_decimal(), 0.0);
	return atof(std::string(data(), length()).c_str());
}
float nstring::to_float() const
{
	f_ret_var(is_decimal(), 0.0f);
	return (float)to_double();
}

bool nstring::is_num() const
{
	if (length() == 0)
		return false;
	for (size_t i = 0; i < length(); i++)
	{
		if (this->operator[](i) < 48 || this->operator[](i) > 57)
		{
			if (i == 0 && this->operator[](i) == '-')
				continue;
			else
				return false;
		}

	}
	return true;
}

bool nstring::is_decimal() const
{
	if (length() == 0)
		return false;
	int point = 0;
	for (size_t i = 0; i < length(); i++)
	{
		if (this->operator[](i) == 46)
			point++;
	}
	if (this->operator[](0) == 46 || this->operator[](length() - 1) == 46 || point >= 2)
		return false;
	return true;
}
//std::vector<nstring_view> nstring_view::split_view(const nstring_view& value) const
//{
//	auto list = find_list(value, 0);
//	std::vector<nstring> result;
//	for (uint32 i = 0; i < list.size(); i++)
//	{
//		if (i == 0)
//		{
//			if (list[i] != 0)
//				result.push_back(nstring(m_data, list[i]));
//		}
//		else
//		{
//			size_t prev_last_idx = list[i - 1] + value.length();
//			if (list[i] != prev_last_idx)
//				result.push_back(nstring(m_data + prev_last_idx, list[i] - prev_last_idx));
//		}
//
//		if (i == list.size() - 1)
//		{
//
//			if (list[i] + value.length() != m_data_length)
//			{
//				size_t last_length = m_data_length - list[i] - value.length();
//				result.push_back(nstring(m_data + list[i] + value.length(), last_length));
//			}
//		}
//	}
//	if (list.size() == 0)
//		result.push_back(*this);
//	return result;
//}
//std::vector<nstring_view> nstring_view::split_view(char value) const
//{
//	return std::vector<nstring_view>();
//}
//std::vector<size_t> nstring_view::find_list_view(const nstring_view& value, size_t start) const
//{
//	return std::vector<size_t>();
//}
size_t nstring_view::strlen(const char* value)
{
	if (value == nullptr)
		return 0;
	return ::strlen(value);
}







nstring::nstring()
{
	
	INIT_STRING;
}
nstring::nstring(size_t block_size)
{
	m_block_size = block_size;
	m_data_length = 0;
	m_mem_length = m_block_size;
	m_data = (char*)mem::malloc(m_block_size);
	m_tail_blank = true;
}
nstring::nstring(const char* value)
{
	INIT_STRING;
	append(value);
}

nstring::nstring(const char* value, size_t len)
{
	INIT_STRING;
	append(value, len);
}

nstring::nstring(const std::string& value)
{
	INIT_STRING;
	append(value);
}

nstring::nstring(const nstring& value)
{
	INIT_STRING;
	append(value);
}
nstring::nstring(const nstring_view& value)
{
	INIT_STRING;
	append(value.data(),value.length());
}
nstring::nstring(const stream_view& value)
{
	INIT_STRING;
	append(value.data(), value.length());
}
#if _HAS_CXX17 || __cplusplus >= 201703L
nstring::nstring(const std::string_view& value)
{
	INIT_STRING;
	append(value.data(), value.length());
}
#endif
#ifdef LIB_QT
nstring::nstring(const QString& value)
{
	INIT_STRING;
	append(value);
}

nstring::nstring(const QByteArray& value)
{
	INIT_STRING;
	append(value.data(), value.length());
}
#endif
nstring::~nstring()
{
	if (m_data != nullptr)
	{
		mem::free(m_data);
	}
}


nstring& nstring::operator=(const char* value)
{
	clear();
	append(value, nstring::strlen(value));
	return *this;
}


nstring& nstring::operator=(const nstring& value)
{
	clear();
	append(value.data(), value.length());
	return *this;
}

nstring& nstring::operator=(const std::string& value)
{
	clear();
	append(value.c_str(), value.length());
	return *this;
}
nstring& nstring::operator=(const nstring_view& value)
{
	clear();
	append(value.data(), value.length());
	return *this;
}
nstring& nstring::operator=(const stream_view& value)
{
	clear();
	append(value.data(), value.length());
	return *this;
}
#if _HAS_CXX17 || __cplusplus >= 201703L
nstring& nstring::operator=(const std::string_view& value)
{
	clear();
	append(value.data(), value.length());
	return *this;
}
#endif
#ifdef LIB_QT
nstring& nstring::operator=(const QString& value)
{
	clear();
	append(value.toLocal8Bit().constData(), value.toLocal8Bit().length());
	return *this;
}
#endif

nstring& nstring::operator+=(const nstring& value)
{
	if (value.length() == 0)return *this;
	append(value);
	return *this;
}

nstring& nstring::operator+=(const char* value)
{
	if (nstring::strlen(value) == 0)return *this;
	append(value);
	return *this;
}
nstring nstring::operator+(const nstring& value) const
{
	if (value.length() == 0)return *this;
	nstring result;
	result.append(*this);
	result.append(value);
	return result;
}

nstring nstring::operator+(const char* value) const
{
	if (value == nullptr || nstring::strlen(value) == 0)return *this;
	nstring result;
	result.append(*this);
	result.append(value);
	return result;
}
bool nstring::operator<(const nstring& right) const
{
	return compare(right) < 0;
}

//void nstring::operator+(const char* value)
//{
//	append(value);
//}
//
//void nstring::operator+(const nstring& value)
//{
//	append(value);
//}

//nstring& nstring::operator+(const nstring& a, const nstring& b)
//{
//	// TODO: 在此处插入 return 语句
//}

void nstring::append(char value, size_t length)
{
	if (length == 0)return;
	char* temp_mem = (char*)mem::malloc(length);
	for (size_t i = 0; i < length; i++)
		temp_mem[i] = value;
	append(temp_mem, length);
	mem::free(temp_mem);
}
void nstring::append(char value)
{
	append(&value, 1);
}
void nstring::append(const char* value)
{
	// TODO: 在此处插入 return 语句
	append(value, nstring::strlen(value));
}
void nstring::append(const nstring& value)
{
	append(value.data(), value.length());
}

void nstring::append(const std::string& value)
{
	append(value.data(), value.length());
}
void nstring::append(const nstring_view& value)
{
	append(value.data(), value.length());
}
void nstring::append(const stream_view& value)
{
	append(value.data(), value.length());
}
#if _HAS_CXX17 || __cplusplus >= 201703L
void nstring::append(const std::string_view& value)
{
	append(value.data(), value.length());
}
#endif
#ifdef LIB_QT
void nstring::append(const QString& value)
{
	if (value.length() == 0)return;
	append(value.toLocal8Bit(), value.toLocal8Bit().length());
}
#endif

nstring nstring::replace(size_t start, size_t len, const nstring& replace_str) const
{
	f_ret_var(lenlegal(start + len, false), "");
	t_ret_var(start >= len, "");
	nstring result;
	result.append(m_data, start);
	result.append(replace_str);
	result.append(m_data + start + len, m_data_length - (start + len));
	return result;
}

nstring nstring::remove(const nstring& value) const
{
	auto list = find_list(value, 0);
	if (list.size() == 0)
		return *this;
	nstring result;
	for (uint32 i = 0; i < list.size(); i++)
	{
		if (i == 0)
		{
			if (list[i] != 0)
				result.append(m_data, list[i]);
		}
		else
		{
			size_t prev_last_idx = list[i - 1] + value.length();
			if (list[i] != prev_last_idx)
				result.append(m_data + prev_last_idx, list[i] - prev_last_idx);
		}

		if (i == list.size() - 1)
		{

			if (list[i] + value.length() != m_data_length)
			{
				size_t last_length = m_data_length - list[i] + value.length();
				result.append(m_data + list[i] + value.length(), last_length);
			}
		}
	}
	return result;
}

nstring nstring::replace(const nstring& replacestring, const nstring& newstring) const
{
	auto list = find_list(replacestring, 0);
	nstring result;
	if (list.size() == 0)
		return *this;
	for (uint32 i = 0; i < list.size(); i++)
	{
		if (i == 0)
		{
			if (list[i] != 0)
			{
				result.append(m_data, list[i]);
				result.append(newstring);
			}

		}
		else
		{
			size_t prev_last_idx = list[i - 1] + replacestring.length();
			if (list[i] != prev_last_idx)
			{
				result.append(m_data + prev_last_idx, list[i] - prev_last_idx);
				result.append(newstring);
			}
		}
		if (i == list.size() - 1)
		{

			if (list[i] + replacestring.length() != m_data_length)
			{
				size_t last_length = m_data_length - list[i] + replacestring.length();
				result.append(m_data + list[i] + replacestring.length(), last_length);
			}
		}
	}
	return result;
}

nstring nstring::replace(char replacestring, const nstring& newstring) const
{
	return	replace(nstring((const char*)&replacestring, 1), newstring);
}

nstring nstring::replace(char replacestring, char newstring) const
{
	return	replace(nstring((const char*)&replacestring, 1), nstring((const char*)&newstring, 1));
}

nstring nstring::replace(const nstring& replacestring, char newstring) const
{
	return	replace(replacestring, nstring((const char*)&newstring, 1));
}


nstring nstring::from(int32 value)
{
#ifdef _WIN32
	char strInt[100];
	memset(strInt, 0, 100);
	itoa(value, strInt, 10);
	return strInt;
#else
	return std::to_string(value);
#endif

}
nstring nstring::from(uint32 value)
{
	return from((int32)value);
}
nstring nstring::from(int64 value)
{
#ifdef WIN32
	char strInt[100];
	memset(strInt, 0, 100);
	return _i64toa(value, strInt, 10);
#else
	return std::to_string(value);
#endif
}

nstring nstring::from(uint64 value)
{
	return from((int64)value);
}

nstring nstring::from(double value)
{

#ifdef _WIN32
	char strInt[100];
	memset(strInt, 0, 100);
	sprintf_s(strInt, 100, "%.2f", value);
	return strInt;
#else
	return std::to_string(value);
#endif
}
#ifndef MSVC_2010
nstring nstring::from(decimal value)
{
	return from(value.to());
}
#endif
nstring nstring::from(const nstring& value)
{
	return value;
}

nstring nstring::from(const std::string& value)
{
	return value.c_str();
}
#ifdef LIB_QT
nstring nstring::from(const QString& value)
{
	return nstring(value.toLocal8Bit());
}

#endif

std::vector<nstring> nstring::split(const nstring_view& value) const
{
	std::vector<stream_view> r = ::stream_view::split_view(value);
	std::vector<nstring> result;
	for (size_t i = 0; i < r.size(); i++)
		result.push_back(r[i]);
	return result;
}

std::vector<nstring> nstring::split(char value) const
{
	std::vector<stream_view> r = ::stream_view::split_view(value);
	std::vector<nstring> result;
	for (size_t i = 0; i < r.size(); i++)
		result.push_back(r[i]);
	return result;
}

void nstring::append(const char* data, size_t length)
{
	if (length == 0 || data == nullptr)
		return;
	// 初始化object
	init_obj();
	if (m_mem_length >= m_data_length + length + 1)
	{
		memcpy((void*)(m_data + m_data_length), data, length);
		m_data_length += length;
		m_data[m_data_length] = 0;
		return;
	}
	size_t new_len;
	if (m_mem_length < length)
		new_len = m_mem_length + length + NSTRING_BLOCKS_SIZE;
	else
		new_len = m_mem_length * 2;
	new_len++;
	if (new_len < m_mem_length + length)
		abort();


	char* new_data = (char*)mem::realloc(m_data, new_len);
	if (new_data == nullptr) {
		new_data = (char*)mem::malloc(new_len);
		memcpy(new_data, m_data, m_data_length);
		memcpy(new_data + m_data_length, data, length);
		mem::free(m_data);
	}
	else {
		if (this->m_data == nullptr) {
			memcpy(new_data, data, length);
		}
		else {
			memcpy(new_data + m_data_length, data, length);
		}

	}
	this->m_data_length = m_data_length + length;
	this->m_mem_length = new_len;
	this->m_data = new_data;
	this->m_data[this->m_data_length] = 0;
}

