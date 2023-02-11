#include "tp_sqlstatement.h"
#include "public/exception.h"
#include <iostream>
#if 1

//#ifdef LIB_DB_MYSQL
#if 1
//#include "core/Core.h"
//#include "core/global.h"
newobj::insert::insert(newobj::db::mysql::pool* pool):isqlstatement(pool)
{
}

newobj::insert::~insert()
{

}

newobj::insert& newobj::insert::table(const nstring& table)
{
	// TODO: 在此处插入 return 语句
	this->m_table = table;
	return *this;
}

newobj::insert& newobj::insert::field(const std::initializer_list<nstring>& fields)
{
	// TODO: 在此处插入 return 语句
	this->m_fields = fields;
	return *this;
}

newobj::insert& newobj::insert::value(const std::initializer_list<newobj::nvar>& values)
{
	// TODO: 在此处插入 return 语句
	this->m_values = values;
	return *this;
}

bool newobj::insert::exec()
{
	if (m_table.empty())
	{
		trw_str("The table name is empty");
	}
	if (m_fields.size() == 0)
	{
		trw_str("The fields is empty");
	}
	if (m_values.size() == 0)
	{
		trw_str("The values is empty");
	}
	if (m_values.size() != m_fields.size())
	{
		trw_str("The number of fields and values is inconsistent");
	}


	int i = 1;
	nstring fields_str;
	nstring wenCode;
	
	/*字段*/
	for (auto field : this->m_fields)
	{
		if (i == this->m_fields.size())
			fields_str += field;
		else
			fields_str += field + ",";
		i++;
	}
	/*内容*/
	for_l (p,this->m_values.size())
	{
		if (p+1 == this->m_values.size())
			wenCode +="?";
		else
			wenCode += "?, ";
	}


	newobj::db::mysql::prepared_statement* pptt = m_conn->sql("INSERT INTO " + this->m_table + "(" + fields_str + ")VALUES(" + wenCode + ")");
	sets(pptt, this->m_values);
	return pptt->update() == 1;
}

void newobj::insert::clear()
{
	this->m_table = "";
	this->m_fields.clear();
	this->m_values.clear();
}

uint64 newobj::insert::last_insert_id()
{
	return m_conn->last_insert_id();
}

newobj::update::update(newobj::db::mysql::pool* pool):isqlstatement(pool)
{

}

newobj::update::~update()
{
}

newobj::update& newobj::update::table(const nstring& table)
{
	// TODO: 在此处插入 return 语句
	this->m_table = table;
	return *this;
}

newobj::update& newobj::update::field(const std::initializer_list<nstring>& fields)
{
	// TODO: 在此处插入 return 语句
	this->m_fields = fields;
	return *this;
}

newobj::update& newobj::update::set(const std::initializer_list<nstring>& sets)
{
	// TODO: 在此处插入 return 语句
	this->m_sets = sets;
	return *this;
}

newobj::update& newobj::update::value(const std::initializer_list<newobj::nvar>& values)
{
	// TODO: 在此处插入 return 语句
	this->m_values = values;
	return *this;
}

newobj::update& newobj::update::where(const newobj::where& where)
{
	// TODO: 在此处插入 return 语句
	this->m_where = where;
	return *this;
}

newobj::update& newobj::update::limit(uint32 start, int32 len)
{
	// TODO: 在此处插入 return 语句
	this->m_limit = " LIMIT " + nstring::from(start) + (len == -2 ? "" : nstring::from(len));
	return *this;
}

uint64 newobj::update::exec()
{
	if (m_table.empty())
	{
		trw_str("The table name is empty");
	}

	int i = 1;
	nstring field_str;
	nstring set_str;
	nstring where_str;

	
	/*字段*/
	for (auto field : this->m_fields)
	{
		if (i == this->m_fields.size())
			field_str += field==""?"":(field+"=?");
		else
			field_str += field == "" ? "" : (field + "=?,");
		i++;
	}
	/*Set*/
	for (auto set : this->m_sets)
	{
		if (i == this->m_sets.size())
			set_str += set+" ";
		else
			set_str += set+" ";
		i++;
	}
	/*查询条件*/
	if (this->m_where.sql != "")
	{
		where_str = "WHERE " + this->m_where.sql;
	}

	db::mysql::prepared_statement* pptt = m_conn->sql("UPDATE  " + this->m_table + " SET " + " " + field_str + " " + set_str + " " + where_str + " " + m_limit);
	sets(pptt, this->m_values);
	sets(pptt, this->m_where.fill, this->m_values.size()+1);
	return pptt->update();
}

void newobj::update::clear()
{
	this->m_table = "";
	this->m_fields.clear();
	this->m_values.clear();
	this->m_where.sql = "";
	this->m_where.fill = {};
	this->m_limit = "";
}

newobj::query::query(newobj::db::mysql::pool* pool):isqlstatement(pool)
{
}

newobj::query::~query()
{
}

newobj::query& newobj::query::table(const nstring& table)
{
	// TODO: 在此处插入 return 语句
	this->m_table = table;
	return *this;
}

//sql::Query& sql::Query::Field(const std::initializer_list<nstring>& fields)
//{
//	// TODO: 在此处插入 return 语句
//	this->m_fields = fields;
//	return *this;
//}
newobj::query& newobj::query::field(const std::vector<nstring>& fields)
{
	// TODO: 在此处插入 return 语句
	this->m_fields = fields;
	return *this;
}
//sql::Query& sql::Query::Where(const sql::Where& where)
//{
//	// TODO: 在此处插入 return 语句
//	this->m_where.fill = where.fill;
//	this->m_where.sql = where.sql;
//	return *this;
//}

newobj::query& newobj::query::where(const where2& where)
{
	// TODO: 在此处插入 return 语句
	this->m_where = where;
	return *this;
}

newobj::query& newobj::query::limit(uint64 start, int len)
{
	// TODO: 在此处插入 return 语句
	this->m_limit = " LIMIT " + nstring::from(start)+(len == -2?"":","+nstring::from(len));
	return *this;
}

newobj::query& newobj::query::orderby(const nstring& orderBy, sort_type sort)
{
	// TODO: 在此处插入 return 语句
	this->m_sort = { orderBy,sort };
	return *this;
}

uint64 newobj::query::exec()
{
	if (m_table.empty())
	{
		trw_str("The table name is empty");
	}
	if (m_fields.size() == 0)
	{
		trw_str("The fields is empty");
	}

	int i = 1;
	nstring field_str;
	nstring where_str;


	/*字段*/
	for (auto field : this->m_fields)
	{
		if (i == this->m_fields.size())
			field_str += field;
		else
			field_str += field+",";
		i++;
	}
	/*查询条件*/
	if (this->m_where.sql != "")
	{
		where_str = "WHERE " + this->m_where.sql;
	}
	/*排序*/
	nstring sort;
	if (this->m_sort.field != "")
	{
		sort = " ORDER BY " + this->m_sort.field + " " + (this->m_sort.type == DESC ? "DESC" : "ASC");
	}
	m_pptt = m_conn->sql("SELECT  " + field_str + " FROM " + this->m_table + " " + where_str + " " + sort + " " + m_limit);
	sets(m_pptt, this->m_where.fill);
	m_result = m_pptt->query();
	return m_result->row();
}

bool newobj::query::next()
{
	return m_result->next();
}

uint64 newobj::query::row()
{
	return m_result->row();
}

nstring newobj::query::to_string(uint32 index)
{
	return m_result->to_string(index);
}

int64 newobj::query::to_int64(uint32 index)
{
	return m_result->to_int64(index);
}

int32 newobj::query::to_int32(uint32 index)
{
	return m_result->to_int32(index);
}
uint64 newobj::query::to_uint64(uint32 index)
{
	return m_result->to_uint64(index);
}
uint32 newobj::query::to_uint32(uint32 index)
{
	return m_result->to_uint32(index);
}
double newobj::query::to_double(uint32 index)
{
	return m_result->to_double(index);
}


json newobj::query::to_json(uint32 index)
{

	newobj::json result;
	result.parse(to_string(index));
	return result;
}



nstring newobj::query::to_string(const nstring& name)
{
	return m_result->to_string(name);
}

int64 newobj::query::to_int64(const nstring& name)
{
	return m_result->to_int64(name);
}

int32 newobj::query::to_int32(const nstring& name)
{
	return m_result->to_int32(name);
}

uint64 newobj::query::to_uint64(const nstring& name)
{
	return m_result->to_uint64(name);
}

uint32 newobj::query::to_uint32(const nstring& name)
{
	return m_result->to_uint32(name);
}

double newobj::query::to_double(const nstring& name)
{
	return m_result->to_double(name);
}
json newobj::query::to_json(const nstring& name)
{
	newobj::json result;
	result.parse(to_string(name));
	return result;
}

void newobj::query::first()
{
	m_result->first();
}

void newobj::query::to_json_array(newobj::json& jsonArray)
{
	jsonArray = newobj::json::array;
	if (m_result->row() == 0)
		return;
	if (m_result->first() == false)
	{
		trw_str("Failed to execute the first() function");
	}
	do
	{
		json line;
		for(uint32 x = 0;x< m_result->column();x++)
		{
			NVarType field_type = m_result->column_type(x + 1);
			if (field_type == NVT_Int)
				line[m_result->column_name(x + 1)] = m_result->to_int32(x + 1);
			else if (field_type == NVT_LLong)
				line[m_result->column_name(x + 1)] = m_result->to_int64(x + 1);
			else if (field_type == NVT_UInt)
				line[m_result->column_name(x + 1)] = m_result->to_int32(x + 1);
			else if (field_type == NVT_ULLong)
				line[m_result->column_name(x + 1)] = m_result->to_uint64(x + 1);
			else if (field_type == NVT_String)
				line[m_result->column_name(x + 1)] = m_result->to_string(x + 1);
			else if (field_type == NVT_Double || field_type == NVT_Float)
				line[m_result->column_name(x + 1)] = m_result->to_double(x + 1);
			else if (field_type == NVT_Boolean)
				line[m_result->column_name(x + 1)] = m_result->to_boolean(x + 1);
		}
		jsonArray.push_back(line);
	} while (m_result->next());
	
}

void newobj::query::to_json_line(json& jsonObj)
{
	for(uint32 x=0;x<m_result->column();x++)
	{
		NVarType field_type = m_result->column_type(x + 1);
		if (field_type == NVT_Int)
			jsonObj[m_result->column_name(x + 1)] = m_result->to_int32(x + 1);
		else if (field_type == NVT_LLong)
			jsonObj[m_result->column_name(x + 1)] = m_result->to_int64(x + 1);
		else if (field_type == NVT_UInt)
			jsonObj[m_result->column_name(x + 1)] = m_result->to_uint32(x + 1);
		else if (field_type == NVT_ULLong)
			jsonObj[m_result->column_name(x + 1)] = m_result->to_uint64(x + 1);
		else if (field_type == NVT_String)
			jsonObj[m_result->column_name(x + 1)] = m_result->to_string(x + 1);
		else if (field_type == NVT_Double || field_type == NVT_Float)
			jsonObj[m_result->column_name(x + 1)] = m_result->to_double(x + 1);
		else if (field_type == NVT_Boolean)
			jsonObj[m_result->column_name(x + 1)] = m_result->to_boolean(x + 1);
	}
}

uint32 newobj::query::column()
{
	return m_result->column();
}

nstring newobj::query::column_name(uint32 index)
{
	return m_result->column_name(index);
}

void newobj::query::clear()
{
	this->m_table = "";
	this->m_fields.clear();
	this->m_where.sql = "";
	this->m_where.fill = {};
	this->m_limit = "";
	this->m_sort.field = "";
}

newobj::del::del(newobj::db::mysql::pool* pool):isqlstatement(pool)
{
}

newobj::del::~del()
{
}

newobj::del& newobj::del::table(const nstring& table)
{
	// TODO: 在此处插入 return 语句
	this->m_table = table;
	return *this;
}

newobj::del& newobj::del::where(const newobj::where& where)
{
	// TODO: 在此处插入 return 语句
	this->m_where = where;
	return *this;
}

newobj::del& newobj::del::limit(uint32 start, int32 len)
{
	// TODO: 在此处插入 return 语句
	this->m_limit = " LIMIT " + nstring::from(start) + (len == -2 ? "" : nstring::from(len));
	return *this;
}

uint64 newobj::del::exec()
{
	if (m_table.empty())
	{
		trw_str("The table name is empty");
	}


	nstring where_str;

	/*查询条件*/
	if (this->m_where.sql != "")
	{
		where_str = "WHERE " + this->m_where.sql;
	}

	newobj::db::mysql::prepared_statement* pptt = m_conn->sql("DELETE FROM  " + this->m_table + " " + where_str + " " + m_limit);
	sets(pptt,this->m_where.fill);
	return pptt->update();
}

void newobj::del::clear()
{
}

void newobj::sets(newobj::db::mysql::prepared_statement* pptt, const std::vector<newobj::nvar>& m_values,int32 start)
{
	for (auto value : m_values)
	{
		switch (value.type())
		{
		case NVT_Int:
			pptt->set_int32(start, value);
			break;
		case NVT_LLong:
			pptt->set_int64(start, value);
			break;
		case NVT_UInt:
			pptt->set_uint32(start, value);
			break;
		case NVT_ULLong:
			pptt->set_uint64(start, value);
			break;
		case NVT_Float:
		case NVT_Double:
			pptt->set_double(start, value);
			break;
		case NVT_String:
			pptt->set_string(start, value);
			break;
		default:
			start--;
		};
		start++;
	}
}
newobj::isqlstatement::isqlstatement(newobj::db::mysql::pool* pool)
{
	m_conn.reset(pool->get());
}

#endif

#endif