#include "basetype/stream.h"
#include "public/exception.h"
#include "util/mem.h"
#include "util/file.h"
#include <string.h>
#include <iostream>
#if 1
#define INIT_STREAM	m_block_size = STREAM_BLOCKS_SIZE;m_data_length = 0;m_mem_length = m_block_size;m_data = (uchar*)mem::malloc(m_block_size);memset(m_data,0,m_block_size)
#define INIT_STREAM_VIEW(VALUE,LENGTH)	m_block_size = 0;m_data_length = LENGTH;m_mem_length = 0;m_data = (uchar*)VALUE
stream_view::stream_view()
{
	INIT_STREAM_VIEW(nullptr, 0);
}
stream_view::stream_view(const char* value, size_t len)
{
	INIT_STREAM_VIEW(value, len);
}
stream_view::~stream_view()
{

}
int stream_view::compare(const stream_view& right) const
{
	auto result = memcmp((const void*)m_data, (const void*)right.data(), std::min(m_data_length, right.length()));
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
bool stream_view::operator<(const stream_view& right) const
{
	return compare(right) < 0;
}
uchar& stream_view::operator[](size_t index) const
{
	lenlegal(index);
	return (uchar)m_data[index];
}

uchar stream_view::at(size_t index) const
{
	return this->operator[](index);
}

bool stream_view::lenlegal(size_t len, bool throw_exp) const
{
	if (len < 0 || len > this->length())
	{
		if (throw_exp)
		{
			//trw_str(stream("length error¡£src:") + stream(*this) + stream(" read:") + stream::from(len));
			trw_str("");
		}
		return false;
	}
	return true;
}

bool stream_view::equals(const char* value, size_t size) const
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

bool stream_view::equals(const stream_view& view) const
{
	return equals(view.data(), view.length());
}


size_t stream_view::find(char value, size_t start_idx) const
{
	return find(stream_view((const char*)&value, 1), start_idx);
}

size_t stream_view::find(const stream_view& value, size_t start_idx) const
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

size_t stream_view::rfind(const stream_view& value) const
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

size_t stream_view::rfind(char value) const
{
	return rfind(stream_view((const char*)&value, 1));
}

std::vector<size_t> stream_view::find_list(const stream_view& value, size_t start) const
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

stream_view stream_view::substr(size_t start, size_t len) const
{
	f_ret_var(lenlegal(start, false), stream_view(nullptr,0));
	f_ret_var(lenlegal(start + len, false), stream_view(nullptr,0));
	return stream_view((char*)m_data+start,len);
}

stream_view stream_view::substr(size_t start) const
{
	f_ret_var(lenlegal(start, false), stream_view(nullptr, 0));
	return  this->substr(start, this->m_data_length - start);
	// ABCDE   1
}

stream_view stream_view::substr(const stream_view& start, const stream_view& end) const
{
	auto idx_start = this->find(start);
	t_ret_var(idx_start == -1, stream_view(nullptr, 0));
	auto idx_end = this->find(end, idx_start);
	t_ret_var(idx_end == -1, stream_view(nullptr, 0));
	return substr(idx_start + start.length(), idx_end - idx_start - start.length());
}


stream_view stream_view::substr(size_t start, const stream_view& end) const
{
	if (start >= length())
		return stream_view(nullptr, 0);
	auto idx_end = this->find(end, start);
	t_ret_var(idx_end == -1, stream_view(nullptr, 0));
	return substr(start, idx_end - start);
}

stream_view stream_view::left(size_t len) const
{
	if (len >= length())
		return *this;
	return substr(0, len);
}

stream_view stream_view::right(size_t len) const
{
	if (len >= length())
		return *this;
	//lenlegal(len);
	return substr(length() - len, len);
}

std::vector<stream_view> stream_view::split_view(const stream_view& value) const
{
	auto list = find_list(value, 0);
	std::vector<stream_view> result;
	for (uint32 i = 0; i < list.size(); i++)
	{
		if (i == 0)
		{
			if (list[i] != 0)
				result.push_back(stream_view((char*)m_data, list[i]));
		}
		else
		{
			size_t prev_last_idx = list[i - 1] + value.length();
			if (list[i] != prev_last_idx)
				result.push_back(stream_view((char*)m_data + prev_last_idx, list[i] - prev_last_idx));
		}

		if (i == list.size() - 1)
		{

			if (list[i] + value.length() != m_data_length)
			{
				size_t last_length = m_data_length - list[i] - value.length();
				result.push_back(stream_view((char*)m_data + list[i] + value.length(), last_length));
			}
		}
	}
	if (list.size() == 0)
		result.push_back(*this);
	return result;
}

std::vector<stream_view> stream_view::split_view(char value) const
{
	return split_view(stream_view((const char*)&value, 1));
}

stream_view stream_view::trim_end(char value) const
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

stream_view stream_view::trim_begin(char value) const
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

stream_view stream_view::trim(char value) const
{
	return trim_begin(value).trim_end(value);
}

bool stream_view::operator==(const stream_view& value) const
{
	return ::stream_view::equals(value.data(), value.length());
}

bool stream_view::operator!=(const stream_view& value) const
{
	return !::stream_view::equals(value.data(), value.length());
}

void stream_view::reset(const char* value, size_t len)
{
	INIT_STREAM_VIEW(value, len);
}
stream::stream()
{
	INIT_STREAM;
}
stream::stream(size_t block_size)
{
	m_block_size = block_size;
	m_data_length = 0;
	m_mem_length = m_block_size;
	m_data = (uchar*)mem::malloc(m_block_size);
}

stream::stream(const char* value, size_t len)
{
	INIT_STREAM;
	append(value, len);
}
stream::stream(const stream_view& value)
{
	INIT_STREAM;
	append(value.data(),value.length());
}

stream::~stream()
{

	if (m_data != nullptr)
	{
		mem::free(m_data);
	}
}
//stream& stream::operator=(const stream_view& value)
//{
//	clear();
//	append(value.data(), value.length());
//	return *this;
//}
//stream& stream::operator+=(const stream_view& value)
//{
//	if (value.length() == 0)return *this;
//	append(value);
//	return *this;
//}
//
//stream stream::operator+(const stream_view& value) const
//{
//	if (value.length() == 0)return *this;
//	stream result;
//	result.append(*this);
//	result.append(value);
//	return result;
//}



void stream::append(char value, size_t length)
{
	if (length == 0)return;
	char* temp_mem = (char*)mem::malloc(length);
	for (size_t i = 0; i < length; i++)
		temp_mem[i] = value;
	append(temp_mem, length);
	mem::free(temp_mem);
}

void stream::append(const stream_view& value)
{
	append(value.data(), value.length());
}
void stream::resize(size_t length)
{
	clear();
	::stream::append((char)0,length);
}
void stream::clear()
{
	m_data_length = 0;
}
void stream::set(size_t start, const stream_view& value)
{
	if (start == 0 || value.length() == 0)
		return;
	lenlegal(start+value.length(),true);
	memcpy(m_data+start,value.data(),value.length());
}
stream stream::replace(size_t start, size_t len, const stream_view& replace_str) const
{
	f_ret_var(lenlegal(start + len, false), stream());
	t_ret_var(start >= len, stream());
	stream result;
	result.append((char*)m_data, start);
	result.append(replace_str);
	result.append((char*)m_data + start + len, m_data_length - (start + len));
	return result;
}

stream stream::remove(const stream_view& value) const
{
	auto list = find_list(value, 0);
	if (list.size() == 0)
		return *this;
	stream result;
	for (uint32 i = 0; i < list.size(); i++)
	{
		if (i == 0)
		{
			if (list[i] != 0)
				result.append((char*)m_data, list[i]);
		}
		else
		{
			size_t prev_last_idx = list[i - 1] + value.length();
			if (list[i] != prev_last_idx)
				result.append((char*)m_data + prev_last_idx, list[i] - prev_last_idx);
		}

		if (i == list.size() - 1)
		{

			if (list[i] + value.length() != m_data_length)
			{
				size_t last_length = m_data_length - list[i] + value.length();
				result.append((char*)m_data + list[i] + value.length(), last_length);
			}
		}
	}
	return result;
}

stream stream::replace(const stream_view& replacestring, const stream_view& newstring) const
{
	auto list = find_list(replacestring, 0);
	stream result;
	if (list.size() == 0)
		return *this;
	for (uint32 i = 0; i < list.size(); i++)
	{
		if (i == 0)
		{
			if (list[i] != 0)
			{
				result.append((char*)m_data, list[i]);
				result.append(newstring);
			}

		}
		else
		{
			size_t prev_last_idx = list[i - 1] + replacestring.length();
			if (list[i] != prev_last_idx)
			{
				result.append((char*)m_data + prev_last_idx, list[i] - prev_last_idx);
				result.append(newstring);
			}
		}
		if (i == list.size() - 1)
		{

			if (list[i] + replacestring.length() != m_data_length)
			{
				size_t last_length = m_data_length - list[i] + replacestring.length();
				result.append((char*)m_data + list[i] + replacestring.length(), last_length);
			}
		}
	}
	return result;
}

stream stream::replace(char replacestring, const stream_view& newstring) const
{
	return	replace(stream((const char*)&replacestring, 1), newstring);
}

stream stream::replace(char replacestring, char newstring) const
{
	return	replace(stream((const char*)&replacestring, 1), stream((const char*)&newstring, 1));
}

stream stream::replace(const stream_view& replacestring, char newstring) const
{
	return	replace(replacestring, stream((const char*)&newstring, 1));
}



std::vector<stream> stream::split(const stream_view& value) const
{
	std::vector<stream_view> r = split_view(value);
	std::vector<stream> result;
	for (size_t i = 0; i < r.size(); i++)
		result.push_back(r[i]);
	return result;
}

std::vector<stream> stream::split(char value) const
{
	std::vector<stream_view> r = split_view(value);
	std::vector<stream> result;
	for (size_t i = 0; i < r.size(); i++)
		result.push_back(r[i]);
	return result;
}
void stream::append(char data)
{
	return append(&data, 1);
}
#define TAIL_BLACK_LENGTH (m_tail_blank==true?1:0)
void stream::append(const char* data, size_t length)
{
	if (length == 0 || data == nullptr)
		return;
	// ³õÊ¼»¯object
	init_obj();
	if (m_mem_length >= m_data_length + length + TAIL_BLACK_LENGTH)
	{
		memcpy((void*)(m_data + m_data_length), data, length);
		m_data_length += length;
		if(m_tail_blank)
			m_data[m_data_length] = 0;
		return;
	}
	size_t new_len;
	if (m_mem_length < length)
		new_len = m_mem_length + length + STREAM_BLOCKS_SIZE;
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
	this->m_data = (uchar*)new_data;
	if (m_tail_blank)
		this->m_data[this->m_data_length] = 0;
}
#endif
