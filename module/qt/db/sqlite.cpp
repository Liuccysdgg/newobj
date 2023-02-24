#include "sqlite.h"
#if LIB_QT == 1
#include "QtCore/qplugin.h"
#include "QtSql/qsqlerror.h"
#include "QtCore/qvariant.h"

newobj::qt::sqlite::sqlite()
{
}

newobj::qt::sqlite::~sqlite()
{
}

bool newobj::qt::sqlite::init(const nstring& path)
{
	____lock____(m_mutex, true);
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName(path);
	t_ret_t(m_db.open());
	m_lastErrorDesc =nstring(m_db.lastError().text());
	return false;
}

void newobj::qt::sqlite::uninit()
{
	____lock____(m_mutex, true);
	m_db.close();
}

bool newobj::qt::sqlite::sql(const QString& sql)
{
	____lock_nolog____(this->m_mutex, true);
	QSqlQuery query(m_db);
	return query.exec(sql);
}

uint32 newobj::qt::sqlite::count(const nstring& sql)
{
	____lock_nolog____(this->m_mutex, true);
	QSqlQuery query(m_db);
	if (query.exec(sql) == false)
		return -1;
	query.next();
	return query.value(0).toInt();
}

QSqlDatabase& newobj::qt::sqlite::database()
{
	return m_db;
}

newobj::mutex* newobj::qt::sqlite::mutex()
{
	return &m_mutex;
}
#endif