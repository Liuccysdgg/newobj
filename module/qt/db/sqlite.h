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
		/*SQLITE���ݿ�ģ��*/
		class sqlite:public base
		{
		public:
			sqlite();
			~sqlite();
			// ��ʼ��
			bool init(const nstring& path);
			// ж��
			void uninit();
			//ִ��SQL
			bool sql(const QString& sql);
			//ȡ���� SELECT COUNT(*) ���
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