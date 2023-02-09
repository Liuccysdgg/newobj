#include "db_sqlite3.h"

bool newobj::db::sqlite3::open(const nstring& filepath, const nstring& username, const nstring& password)
{
	m_db = nullptr;
	if (sqlite3_open(filepath.c_str(), &m_db) != 0)
	{
		this->m_lastErrorDesc = sqlite3_errmsg(m_db);
		return false;
	}
	return true;
}

void newobj::db::sqlite3::close()
{
	sqlite3_close(m_db);
}

bool newobj::db::sqlite3::exec(const nstring& sql)
{
	return sqlite3_exec(m_db,sql.c_str(),nullptr,nullptr,nullptr) == SQLITE_OK;
}
static int QueryResult(void* param, int column, char** argv, char** azColName)
{
	std::vector<std::map<nstring, nstring>>* data = (std::vector<std::map<nstring, nstring>>*)param;
	std::map<nstring, nstring> line;
	for (int i = 0; i < column; i++)
	{
		line[azColName[i]] = argv[i] == NULL ? "" : argv[i];
	}
		
	data->push_back(line);
	return 0;
}
bool newobj::db::sqlite3::query(const nstring& sql, std::vector<std::map<nstring, nstring>>& data)
{
	char* cErrMsg = nullptr;
	if (sqlite3_exec(m_db, sql.c_str(), QueryResult, (void*)&data, &cErrMsg) != SQLITE_OK)
	{
		this->m_lastErrorDesc = cErrMsg;
		return false;
	}
	return true;
}
