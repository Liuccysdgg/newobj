#include "package.h"
#include "util/bytes.h"
#include "util/codec.h"
#define CHECK_DATA_LENGTH(DATA,LENGTH) if (DATA->length() < (LENGTH))return false;
newobj::package::package()
{
}

newobj::package::~package()
{
	free();
}

bool newobj::package::parse(const newobj::buffer& data, const nstring& password)
{
	free();
	if (data.length() == 0)
		return false;
	uint32 idx = 0;
	while (true)
	{
		
		nstring name;
		newobj::buffer* signal = nullptr;
		{
			/*名称解析*/
			uint32 name_length = 0;
			if (data.length() < idx + 4)
				return false;
			bytes::to_uint(name_length, data.data()+idx);
			idx += 4;
			if (data.length() < idx + name_length)
				return false;
			name = data.sub(idx, name_length).to_string();
			idx += name_length;
		}
		{
			/*数据解析*/
			uint32 data_length = 0;
			if (data.length() < idx + 4)
				return false;
			bytes::to_uint(data_length, data.data()+idx);
			idx += 4;
			if (data.length() < idx + data_length)
				return false;
			signal = new newobj::buffer;
			*signal = data.sub(idx, data_length);
			idx += data_length;
		}
		m_parse_list.add(name,signal);
		if (idx == data.length())
			break;
	}
	return true;
}

bool newobj::package::add(const nstring& name, const nvar& data, const nstring& password)
{
	t_ret_f(m_list.exist(name));
	packstruct* ps = new packstruct;
	ps->var = true;
	ps->name = name;
	ps->password = password;
	ps->data_var = data;
	m_list.add(name,ps);
	return true;
}

bool newobj::package::add(const nstring& name, const newobj::buffer& data, const nstring& password)
{
	t_ret_f(m_list.exist(name));
	packstruct* ps = new packstruct;
	ps->var = false;
	ps->name = name;
	ps->password = password;
	ps->data_buffer = data;
	m_list.add(name, ps);
	return true;
}


void newobj::package::free()
{
	/*释放合成资源*/
	{
		auto list = m_list.to_stl();
		for_iter(iter, list)
		{
			delete iter->second;
		}
		m_list.clear();
	}
	/*释放解析资源*/
	{
		auto list = m_parse_list.to_stl();
		for_iter(iter, list)
		{
			delete iter->second;
		}
		m_parse_list.clear();
	}
}

void newobj::package::to(const nstring& password, newobj::buffer& data)
{
	// 0=32整数 1=double浮点  2=文本型 3=字节流
	auto list  = m_list.to_stl();
	uint32 count = list.size();
	data.clear();
	for_iter(iter, list)
	{
		newobj::buffer signal_name;
		newobj::buffer signal_data;
		// 填充名称
		{
			signal_name.append(bytes::to_buffer((int32)iter->second->name.length()));
			signal_name.append(iter->second->name);
		}
		// 填充数据
		if(iter->second->var)
		{
			switch (iter->second->data_var.type())
			{
			case NVT_Int:
			case NVT_UInt:
			case NVT_Long:
			case NVT_ULong:
			case NVT_Short:
			case NVT_UShort:
				signal_data.append((uchar)0);
				signal_data.append(bytes::to_buffer((int32)4));
				signal_data.append(bytes::to_buffer((int32)iter->second->data_var.to_int32()));
				break;
			case NVT_LLong:
			case NVT_ULLong:
			case NVT_Double:
			case NVT_Float:
				signal_data.append((uchar)1);
				signal_data.append(bytes::to_buffer((int32)8));
				signal_data.append(bytes::to_buffer((int32)iter->second->data_var.to_uint64()));
				break;
			case NVT_String:
				signal_data.append((uchar)2);
				signal_data.append(bytes::to_buffer((int32)iter->second->data_var.to_string().length()));
				signal_data.append(iter->second->data_var.to_string());
				break;
			default:
				break;
			}
		}
		else
		{
			signal_data.append((uchar)3);
			signal_data.append(bytes::to_buffer((int32)iter->second->data_buffer.length()));
			signal_data.append(iter->second->data_buffer);
		}
		// 打包加密
		{
			nstring iter_pwd;
			if (iter->second->password.length() != 0)
				iter_pwd = iter->second->password;
			else if (m_defaultpassword.length() != 0)
				iter_pwd = m_defaultpassword;

			if(iter_pwd.length() != 0)
				signal_data = codec::des::en(signal_data,iter->second->password);

			data.append(signal_name);
			data.append(bytes::to_buffer((int32)signal_data.length()));
			data.append(signal_data);
		}
	}
}
bool newobj::package::get(const nstring& name, nvar& data, const nstring& password)
{
	newobj::buffer* signal = nullptr;
	f_ret_f(m_parse_list.get(name, signal));
	if (password != "")
		*signal = codec::des::de(*signal,password);
	uint32 idx = 0;
	CHECK_DATA_LENGTH(signal, idx + 1);
	// 0=32整数 1=double浮点  2=文本型 3=字节流
	uint32 type = (*signal)[idx];
	idx++;
	CHECK_DATA_LENGTH(signal, idx + 4);
	uint32 length = 0;
	bytes::to_uint(length,signal->data()+idx);
	idx += 4;
	if (type == 0)
	{
		uint32 s_data = 0;
		CHECK_DATA_LENGTH(signal, idx + length);
		data.type(NVT_Int);
		bytes::to_uint(s_data, signal->data() + idx);
		data = s_data;
	}
	else if (type == 1)
	{
		double s_data = 0;
		CHECK_DATA_LENGTH(signal, idx + length);
		data.type(NVT_Int);
		memcpy((void*)&s_data,signal->data()+idx,length);
		data = s_data;
	}
	return false;
}

bool newobj::package::get(const nstring& name, newobj::buffer& data, const nstring& password)
{
	newobj::buffer* signal = nullptr;
	f_ret_f(m_parse_list.get(name, signal));
	if (password != "")
		*signal = codec::des::de(*signal, password);
	uint32 idx = 0;
	CHECK_DATA_LENGTH(signal, idx + 1);
	// 0=32整数 1=double浮点  2=文本型 3=字节流
	uint32 type = (*signal)[idx];
	idx++;
	CHECK_DATA_LENGTH(signal, idx + 4);
	uint32 length = 0;
	bytes::to_uint(length, signal->data() + idx);
	idx += 4;
	if (type == 3)
	{
		CHECK_DATA_LENGTH(signal, idx + length);
		data = signal->sub(idx,length);
		return true;
	}
	return false;
}

void newobj::package::setdefaultpassword(const nstring& password)
{
	m_defaultpassword = password;
}

void newobj::package::clear()
{
	free();
}
