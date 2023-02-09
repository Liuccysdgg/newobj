#pragma once
#include "public/string.h"
#include "QtSql/qsqldatabase.h"
#include "QtSql/qsqlquery.h"
#include "util/lock.h"
#include "public/base.h"
namespace newobj
{
	namespace qt
	{
		/*SQLITE数据库模块*/
		class sqlite:public base
		{
		public:
			sqlite();
			~sqlite();
			// 初始化
			bool init(const nstring& path);
			// 卸载
			void uninit();
			//执行SQL
			bool sql(const QString& sql);
			//取数量 SELECT COUNT(*) 语句
			uint32 count(const nstring& sql);
		public:
			QSqlDatabase& database();
			newobj::mutex* mutex();
		protected:
			QSqlDatabase m_db;
			newobj::mutex m_mutex;
		};
	}
	
}