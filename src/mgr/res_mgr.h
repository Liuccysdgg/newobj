#pragma once
#ifndef MSVC_2010
#include "public/define.h"
#include "public/string.h"
#include "public/buffer.h"
#include "public/struct.h"
#include "util/json.h"
#include <functional>

#if 0
#ifdef WIN32
#include "leveldb/include/leveldb/db.h"
#else
#include "leveldb/include/leveldb/db.h"
#endif



namespace newobj
{
	namespace mgr
	{
		/**********************************************************
		 * Class：数据资源管理器
		 **********************************************************/
		class res
		{
		public:
			res(const nstring& path);
			~res();
			bool open(bool create_if_missing = true);
			void close();
			nstring path();
			void encrypt(newobj::Endecrypt type,const nstring& password);
			bool read(const nstring& type,const nstring& name, newobj::buffer& data);
			bool read(const nstring& type, const nstring& name, newobj::json& data);
			bool read(const nstring& type, const nstring& name, int32& data);
			bool read(const nstring& type, const nstring& name, int64& data);
			bool read(const nstring& type, const nstring& name, nstring& data);
			newobj::buffer read(const nstring& type, const nstring& name,std::function<void()> callback_noread);


			bool write(const nstring& type,const nstring& name,const newobj::buffer& data);
			bool write(const nstring& type, const nstring& name, const newobj::json& data);
			bool write(const nstring& type, const nstring& name, const int32& data);
			bool write(const nstring& type, const nstring& name, const int64& data);
			bool write(const nstring& type, const nstring& name, const nstring& data);
		private:
			/*加密信息*/
			newobj::Endecrypt m_ed_type;
			nstring m_ed_password;
			// 目录
			nstring m_path;
			// DB
			leveldb::DB* m_db = nullptr;
		};
	}
}
#endif
#endif