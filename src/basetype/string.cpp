#include "string.h"
#include "public/exception.h"
#include "util/mem.h"
#include "util/file.h"
#include <string.h>
#include <iostream>
char nstring_view::m_empty_string[1] = { '\0' };
#define INIT_STRING	m_block_size = NSTRING_BLOCKS_SIZE;m_data_length = 0;m_mem_length = m_block_size;m_data = (char*)mem::malloc(m_block_size);memset(m_data,0,m_block_size)

#define INIT_STRING_VIEW(VALUE,LENGTH)	m_block_size = 0;m_data_length = LENGTH;m_mem_length = 0;m_data = (char*)VALUE

nstring_view::nstring_view()
{
	INIT_STRING_VIEW(nullptr, 0);
}
nstring_view::nstring_view(const std::string& value)
{
	INIT_STRING_VIEW(value.c_str(), value.length());
}
nstring_view::nstring_view(const char* value)
{
	INIT_STRING_VIEW(value, strlen(value));
}
nstring_view::nstring_view(const char* value, size_t len)
{
	INIT_STRING_VIEW(value, len);
}
nstring_view::~nstring_view()
{

}
int nstring_view::compare(const nstring_view& right) const
{
	auto result = memcmp((const void*)m_data, (const void*)right.c_str(), std::min(m_data_length, right.length()));
	if (result != 0) {
		return result;
	}
	if (m_data_length < right.length()) {
		return -1;
	}
	if (m_data_length > right.length()) {
		return 1;
	}
	return 0;
}
char& nstring_view::operator[](size_t index) const
{
	lenlegal(index);
	return m_data[index];
}

char nstring_view::at(size_t index) const
{
	return this->operator[](index);
}

bool nstring_view::lenlegal(size_t len, bool throw_exp) const
{
	if (len < 0 || len > this->length())
	{
		if (throw_exp)
		{
			//trw_str(nstring("length error。src:") + nstring(*this) + nstring(" read:") + nstring::from(len));
			trw_str("");
		}
		return false;
	}
	return true;
}

bool nstring_view::equals(const char* value, size_t size) const
{
	t_ret_t(size == 0 && length() == 0);
	t_ret_t(value == nullptr && length() == 0);
	f_ret_f(size == length())


		for (size_t i = 0; i < size; i++)
		{
			if (m_data[i] != value[i])
				return false;
		}
	return true;
}
nstring_view::operator std::string() const
{
	return this->c_str();
}
#ifdef LIB_QT
nstring_view::operator QString() const
{
	return this->c_str();
}
nstring_view::operator QByteArray() const
{
	return operator QString().toLocal8Bit();
}
#endif
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
		return atoi(this->c_str());
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
		return _atoi64(this->c_str());
#else
		return atoll(this->c_str());
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
	return decimal(atof(this->c_str()));
}
#endif
double nstring::to_double() const
{
	f_ret_var(is_decimal(), 0.0);
	return atof(this->c_str());
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

size_t nstring_view::find(char value, size_t start_idx) const
{
	return find(nstring_view((const char*)&value, 1), start_idx);
}

size_t nstring_view::find(const nstring_view& value, size_t start_idx) const
{
	try
	{
		for (size_t i = start_idx; i < m_data_length; i++)
		{
			bool find = true;
			for (size_t x = 0; x < value.length(); x++)
			{
				if (m_data[i + x] != value[x])
				{
					find = false;
					break;
				}
			}
			if (find == true)
				return i;
		}
	}
	catch (const std::exception&)
	{
		return -1;
	}

	return -1;
}

size_t nstring_view::rfind(const nstring_view& value) const
{
	try
	{
		for (size_t i = m_data_length - 1; i > 0; i--)
		{
			bool find = true;
			for (size_t x = value.length(); x > 0; x--)
			{
				if (m_data[i - (value.length() - x)] != value[x - 1])
				{
					find = false;
					break;
				}
			}
			if (find == true)
				return i - value.length() + 1;
		}
	}
	catch (const std::exception&)
	{
		return -1;
	}

	return -1;
}

size_t nstring_view::rfind(char value) const
{
	return rfind(nstring_view((const char*)&value, 1));
}

std::vector<size_t> nstring_view::find_list(const nstring_view& value, size_t start) const
{
	std::vector<size_t> result;
	size_t idx = 0;
	while (idx != -1)
	{
		idx = find(value, start == 0 ? 0 : start + 1);
		if (idx == -1)
			break;
		result.push_back(idx);
		start = idx + 1;
	}
	return result;
}

nstring_view nstring_view::substr(size_t start, size_t len) const
{
	f_ret_var(lenlegal(start, false), "");
	f_ret_var(lenlegal(start + len, false), "");
	return nstring_view(m_data+start,len);
}

nstring_view nstring_view::substr(size_t start) const
{
	f_ret_var(lenlegal(start, false), "");
	return  this->substr(start, this->m_data_length - start);
	// ABCDE   1
}

nstring_view nstring_view::substr(const nstring_view& start, const nstring_view& end) const
{
	auto idx_start = this->find(start);
	t_ret_var(idx_start == -1, "");
	auto idx_end = this->find(end, idx_start);
	t_ret_var(idx_end == -1, "");
	return substr(idx_start + start.length(), idx_end - idx_start - start.length());
}


nstring_view nstring_view::substr(size_t start, const nstring_view& end) const
{
	if (start >= length())
		return "";
	auto idx_end = this->find(end, start);
	t_ret_var(idx_end == -1, "");
	return substr(start, idx_end - start);
}

nstring_view nstring_view::left(size_t len) const
{
	if (len >= length())
		return *this;
	return substr(0, len);
}

nstring_view nstring_view::right(size_t len) const
{
	if (len >= length())
		return *this;
	//lenlegal(len);
	return substr(length() - len, len);
}

std::vector<nstring_view> nstring_view::split(const nstring_view& value) const
{
	auto list = find_list(value, 0);
	std::vector<nstring_view> result;
	for (uint32 i = 0; i < list.size(); i++)
	{
		if (i == 0)
		{
			if (list[i] != 0)
				result.push_back(nstring_view(m_data, list[i]));
		}
		else
		{
			size_t prev_last_idx = list[i - 1] + value.length();
			if (list[i] != prev_last_idx)
				result.push_back(nstring_view(m_data + prev_last_idx, list[i] - prev_last_idx));
		}

		if (i == list.size() - 1)
		{

			if (list[i] + value.length() != m_data_length)
			{
				size_t last_length = m_data_length - list[i] - value.length();
				result.push_back(nstring_view(m_data + list[i] + value.length(), last_length));
			}
		}
	}
	if (list.size() == 0)
		result.push_back(*this);
	return result;
}

std::vector<nstring_view> nstring_view::split(char value) const
{
	return split(nstring_view((const char*)&value, 1));
}

nstring_view nstring_view::trim_end(char value) const
{
	uint32 idx = 0;
	for (uint32 i = 0; i < length(); i++)
	{
		if (operator[](length() - i - 1) == value)
			idx++;
		else
			break;
	}
	return this->left(length() - idx);
}

nstring_view nstring_view::trim_begin(char value) const
{
	uint32 idx = 0;
	for (uint32 i = 0; i < length(); i++)
	{
		if (operator[](i) == value)
			idx++;
		else
			break;
	}
	return this->right(length() - idx);
}

nstring_view nstring_view::trim(char value) const
{
	return trim_begin(value).trim_end(value);
}

bool nstring_view::operator==(const char* value)  const
{
	return ::nstring_view::equals(value, ::nstring_view::strlen(value));
}
bool nstring_view::operator==(const std::string& value) const
{
	return ::nstring_view::equals(value.c_str(), value.length());
}
bool nstring_view::operator==(const nstring_view& value) const
{
	return ::nstring_view::equals(value.c_str(), value.length());
}
bool nstring_view::operator!=(const char* value) const
{
	return !::nstring_view::equals(value, nstring::strlen(value));
}

bool nstring_view::operator!=(const nstring_view& value) const
{
	return !::nstring_view::equals(value.c_str(), value.length());
}

bool nstring_view::operator!=(const std::string& value) const
{
	return !::nstring_view::equals(value.c_str(), value.length());
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
	append(value.c_str(),value.length());
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
	append(value.c_str(), value.length());
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
	append(value.c_str(), value.length());
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

	//return m_length < left.length();
	//return !equal(left.c_str(),left.length());
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
//
//void nstring::append(const char* value, size_t length)
//{
//	if (value == nullptr || length == 0)
//		return;
//	size_t new_len = m_length + length;
//	char* new_data = (char*)mem::realloc(m_data, new_len + 1);
//	if (new_data == nullptr) {
//		new_data = (char*)mem::malloc(new_len);
//		memcpy(new_data, m_data, m_length);
//		memcpy(new_data + m_length, value, length);
//		mem::free(m_data);
//	}
//	else {
//		if (this->m_data == nullptr) {
//			memcpy(new_data, value, length);
//		}
//		else {
//			memcpy(new_data + m_length, value, length);
//		}
//
//	}
//	this->m_data = new_data;
//	this->m_length = new_len;
//	this->m_data[m_length] = 0;
//}

void nstring::append(const nstring& value)
{
	append(value.c_str(), value.length());
}

void nstring::append(const std::string& value)
{
	append(value.c_str(), value.length());
}
void nstring::append(const nstring_view& value)
{
	append(value.c_str(), value.length());
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

std::vector<nstring> nstring::splitn(const nstring_view& value) const
{
	std::vector<nstring_view> r = split(value);
	std::vector<nstring> result;
	for (size_t i = 0; i < r.size(); i++)
		result.push_back(r[i]);
	return result;
}

std::vector<nstring> nstring::splitn(char value) const
{
	std::vector<nstring_view> r = split(value);
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

