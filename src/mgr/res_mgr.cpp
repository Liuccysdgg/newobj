
#include "res_mgr.h"
#if 0
#include "util/codec.h"
#ifndef MSVC_2010
newobj::mgr::res::res(const nstring& path)
{
	m_path = path;
	m_ed_type = newobj::Endecrypt::ED_NONE;
}

newobj::mgr::res::~res()
{
	close();
}
bool newobj::mgr::res::open(bool create_if_missing)
{
	if (m_db != nullptr)
		return false;
	leveldb::Options options;
	options.create_if_missing = create_if_missing;
	if (leveldb::DB::Open(options, m_path, &m_db).ok() == false)
	{
		m_db = nullptr;
		return false;
	}
	return true;
}
void newobj::mgr::res::close()
{
	if (m_db != nullptr)
	{
		delete m_db;
		m_db = nullptr;
	}
}
nstring newobj::mgr::res::path()
{
	return m_path;
}

void newobj::mgr::res::encrypt(newobj::Endecrypt type, const nstring& password)
{
	this->m_ed_type = type;
	this->m_ed_password = password;
}

bool newobj::mgr::res::read(const nstring& type, const nstring& name, newobj::buffer& data)
{
	std::string read;
	if (m_db->Get(leveldb::ReadOptions(), nstring(type+"__leelbky__"+name).c_str(), &read).ok() == false)
		return false;
	data = codec::base64::de(read);
	if (m_ed_type == ED_AES)
		data = codec::aes::de(data,m_ed_password);
	else if (m_ed_type == ED_DES3)
		data = codec::des::de(data, m_ed_password);
	return true;
}

bool newobj::mgr::res::read(const nstring& type, const nstring& name, newobj::json& data)
{
	nstring sdata;
	f_ret_f(read(type, name, sdata));
	data.parse(sdata);
	return true;
}

bool newobj::mgr::res::read(const nstring& type, const nstring& name, int32& data)
{
	nstring sdata;
	f_ret_f(read(type, name, sdata));
	data =  sdata.to_int32();
	return true;
}

bool newobj::mgr::res::read(const nstring& type, const nstring& name, int64& data)
{
	nstring sdata;
	f_ret_f(read(type, name, sdata));
	data = sdata.to_int64();
	return true;
}

bool newobj::mgr::res::read(const nstring& type, const nstring& name, nstring& data)
{
	newobj::buffer bdata;
	f_ret_f(read(type, name, bdata));
	data = bdata.to_string();
	return true;
}

newobj::buffer newobj::mgr::res::read(const nstring& type, const nstring& name, std::function<void()> callback_noread)
{
	newobj::buffer data;
	if (read(type, name, data))
		return data;
	callback_noread();
	return data;
}

bool newobj::mgr::res::write(const nstring& type, const nstring& name, const newobj::buffer& data)
{
	leveldb::WriteOptions putOptions;
	putOptions.sync = true;
	newobj::buffer write_data;
	if (m_ed_type == ED_AES)
		write_data = codec::aes::en(data, m_ed_password);
	else if (m_ed_type == ED_DES3)
		write_data = codec::des::en(data, m_ed_password);
	else
		write_data = data;
	return m_db->Put(putOptions, nstring(type + "__leelbky__" + name).c_str(), codec::base64::en(write_data).c_str()).ok();
}

bool newobj::mgr::res::write(const nstring& type, const nstring& name, const newobj::json& data)
{
	return write(type,name,newobj::buffer(data.to_string()));
}

bool newobj::mgr::res::write(const nstring& type, const nstring& name, const int32& data)
{
	return write(type, name, newobj::buffer(nstring::from(data)));
}

bool newobj::mgr::res::write(const nstring& type, const nstring& name, const int64& data)
{
	return write(type, name, newobj::buffer(nstring::from(data)));
}

bool newobj::mgr::res::write(const nstring& type, const nstring& name, const nstring& data)
{
	return write(type, name, newobj::buffer(data));
}
#endif
#endif