#pragma once
#include "public/define.h"
#include "public/string.h"
#include "public/base.h"
#include "../thirdparty/sqlite3/sqlite3.h"
#include <map>
#define SQLITE_RESULT std::vector<std::map<nstring, nstring>>
namespace newobj
{
	namespace db
	{
		class sqlite3:public newobj::base
		{
		public:
			bool open(const nstring& filepath,const nstring& username ="",const nstring& password = "");
			void close();
			bool exec(const nstring& sql);
			bool query(const nstring& sql, std::vector<std::map<nstring, nstring>>& data);
		private:
			struct ::sqlite3* m_db;
		};
	}
}