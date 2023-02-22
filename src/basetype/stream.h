#pragma once
#include "public/define.h"
#include "public/object.hpp"
#include <vector>
#define STREAM_BLOCKS_SIZE 128

class NEWOBJ_API stream_view :public object
{
public:
	stream_view();

	stream_view(const char* value, size_t len);
	~stream_view();

	bool operator==(const stream_view& value) const;

	inline void clear() { m_data_length = 0; }
	inline size_t length() const { return m_data_length; }
	inline bool empty() const { return m_data == nullptr || m_data_length == 0; }
	char& operator[](size_t index) const;
	char at(size_t index) const;


	bool equals(const char* value, size_t size) const;
	bool equals(const stream_view& view) const;

	size_t find(char value, size_t start_idx = 0) const;
	size_t find(const stream_view& value, size_t start_idx = 0) const;
	size_t rfind(const stream_view& value) const;
	size_t rfind(char value) const;
	std::vector<size_t> find_list(const stream_view& value, size_t start = 0) const;
	stream_view substr(size_t start, size_t len) const;
	stream_view substr(size_t start) const;
	stream_view substr(const stream_view& start, const stream_view& end) const;
	stream_view substr(size_t start, const stream_view& end) const;
	stream_view left(size_t len) const;
	stream_view right(size_t len) const;
	std::vector<stream_view> split(const stream_view& value)  const;
	std::vector<stream_view> split(char value)  const;



	stream_view trim_end(char value) const;
	stream_view trim_begin(char value) const;
	stream_view trim(char value) const;

protected:
	int compare(const stream_view& right) const;
	bool lenlegal(size_t len, bool throw_exp = true) const;
protected:
	static char m_empty_string[1];
	char* m_data;
	size_t m_data_length;
	size_t m_mem_length;
	size_t m_block_size;
};

class NEWOBJ_API stream :public stream_view
{
public:
	stream();
	stream(size_t block_size);
	stream(const char* value, size_t len);
	stream(const stream_view& value);
	~stream();

	stream& operator=(const stream_view& value);
	stream& operator+=(const stream_view& value);

	std::vector<stream_view> splitn(const stream_view& value)  const;
	std::vector<stream_view> splitn(char value)  const;
	void append(const char* data, size_t length);
	void append(const stream_view& value);
	
	stream remove(const stream_view& value) const;



	stream replace(size_t start, size_t len, const stream_view& value) const;
	stream replace(const stream_view& replacestring, const stream_view& newstring) const;
	stream replace(char replacestring, const stream_view& newstring) const;
	stream replace(char replacestring, char newstring) const;
	stream replace(const stream_view& replacestring, char newstring) const;


};
