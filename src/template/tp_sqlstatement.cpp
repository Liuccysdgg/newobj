#include "template/tp_sqlstatement.h"
#include "public/exception.h"
#include "util/time.h"
#include <iostream>
#ifndef MSVC_2010
inline size_t sets(newobj::mysql_plus::prepare_statement *ppst,size_t start_index,const std::vector<std::any>& values){
    size_t index =start_index;
    for(size_t i=0;i<values.size();i++){
        if(typeid(nstring) == values[i].type())
            ppst->set_string(index++,std::any_cast<nstring>(values[i]));
        else if(typeid(int32) == values[i].type())
            ppst->set_int32(index++,std::any_cast<int32>(values[i]));
        else if(typeid(uint32) == values[i].type())
            ppst->set_string(index++,std::any_cast<uint32>(values[i]));
        else if(typeid(nstring) == values[i].type())
            ppst->set_string(index++,std::any_cast<int64>(values[i]));
        else if(typeid(nstring) == values[i].type())
            ppst->set_string(index++,std::any_cast<uint64>(values[i]));
        else if(typeid(float) == values[i].type())
            ppst->set_double(index++,std::any_cast<float>(values[i]));
        else if(typeid(double) == values[i].type())
            ppst->set_double(index++,std::any_cast<double>(values[i]));
        else if(typeid(int64) == values[i].type())
            ppst->set_bigint(index++,nstring::from(std::any_cast<int64>(values[i])));
        else if(typeid(uint64) == values[i].type())
            ppst->set_bigint(index++,nstring::from(std::any_cast<uint64>(values[i])));
        else if(typeid(time::datetime) == values[i].type())
        {
            time::datetime dt = std::any_cast<time::datetime>(values[i]);
            ppst->set_datetime(index++,time::format(newobj::time::to_ts(dt)));
        }

        else if(typeid(bool) == values[i].type())
            ppst->set_boolean(index++,std::any_cast<bool>(values[i]));
        else if(typeid(void) == values[i].type())
        {
            std::cout<<"index:"<<index<<" std::any is void"<<std::endl;
        }
        else
        {
            trw_str("unknown type");
        }
    }
    return index;
}
inline void make_where(nstring& sql,const std::vector<sql::where>& wheres)
{
    if(wheres.size() != 0)
    {
        bool insert_where = false; 
        sql.append(" WHERE ");
        for(size_t i=0;i<wheres.size();i++)
        {
            if(wheres[i].value.type() == typeid(void))
                continue;
        
            if(insert_where)
                sql.append(",");
            insert_where = true;
            sql.append(wheres[i].name);
            sql.append(wheres[i].code);
            sql.append("?");
        }
    }

}
newobj::sql::insert::insert()
{

}
newobj::sql::insert::~insert()
{

}
newobj::sql::insert& newobj::sql::insert::table(const nstring& table)
{
	this->m_table = table;
	return *this;
}
newobj::sql::insert& newobj::sql::insert::field(const std::initializer_list<nstring>& fields)
{
	this->m_fields = fields;
	return *this;
}

newobj::sql::insert& newobj::sql::insert::value(const std::initializer_list<std::any>& values)
{
	this->m_values.push_back(values);
	return *this;
}

newobj::sql::insert& newobj::sql::insert::exec(newobj::mysql_plus::conn* conn)
{
	if (m_table.empty())
	{
		trw_str("table name is empty");
	}
	if (m_fields.size() == 0)
	{
		trw_str("fields is empty");
	}
	if (m_values.size() == 0)
	{
		trw_str("values is empty");
	}
	nstring fields_str;
	nstring value_code;
	/*×Ö¶Î*/
    for (size_t i=0;i<m_fields.size();i++){
        fields_str.append(m_fields[i]);
        fields_str.append(',');
    }
        
	/*ÄÚÈÝ*/
	for (size_t i=0;i<this->m_values.size();i++)
	{
        if(!value_code.empty())
            value_code.append(',');
        value_code.append('(');
        auto sig = this->m_values[i];
        for(size_t f=0;f<sig.size();f++){
            if(f+1 == sig.size())
    		    value_code +="?";
            else
                value_code += "?,";
        }
        value_code.append(')');
	}
    fields_str = fields_str.trim_end(',');
    nstring sql(fields_str.length()+value_code.length()+100);

    sql.append("INSERT INTO ");
    sql.append(this->m_table);
    sql.append("(");
    sql.append(fields_str);
    sql.append(")VALUES");
    sql.append(value_code);

    auto ppst = conn->setsql(sql);
    size_t index = 1;
    for(size_t i=0;i<m_values.size();i++)
        index = sets(ppst,index, this->m_values[i]);
    this->m_affected_count =  ppst->update();
    return *this;
}

void newobj::sql::insert::clear()
{
	this->m_table = "";
	this->m_fields.clear();
	this->m_values.clear();
    this->m_affected_count = 0;
}


newobj::sql::update::update()
{

}
newobj::sql::update::~update()
{

}
newobj::sql::update& newobj::sql::update::table(const nstring& table)
{
    this->m_table = table;
    return *this;
}
newobj::sql::update& newobj::sql::update::field(const std::initializer_list<nstring>& fields)
{

    this->m_fields = fields;
    return *this;
}
newobj::sql::update& newobj::sql::update::where(const std::initializer_list<sql::where>& wheres)
{

    this->m_wheres = wheres;
    return *this;
}
newobj::sql::update& newobj::sql::update::set(const std::initializer_list<sql::value>& values)
{
    this->m_values = values;
    return *this;
}
newobj::sql::update& newobj::sql::update::limit(uint64 start,uint64 len)
{
    this->m_limit.start = start;
    this->m_limit.count = len;
    return *this;
}
newobj::sql::update& newobj::sql::update::exec(newobj::mysql_plus::conn* conn)
{
    nstring sql(1024);
    sql.append("UPDATE ");
    sql.append(m_table);
    sql.append("SET ");
    for(size_t i=0;i<m_values.size();i++)
    {
        sql.append(m_values[i].name);
        sql.append("=?");
    }
    make_where(sql,m_wheres);
    if(m_limit.count != 0)
        sql.append(" LIMIT "+nstring::from(m_limit.start)+","+nstring::from(m_limit.count));

    std::vector<std::any> any_value;
    std::vector<std::any> any_where;
    for(size_t i=0;i<m_values.size();i++)
        any_value.push_back(m_values[i].val);
    for(size_t i=0;i<m_wheres.size();i++)
    {
        if(m_wheres[i].value.type() == typeid(void))
            continue;
        any_where.push_back(m_wheres[i].value);
    }
    auto ppts = conn->setsql(sql);
    size_t index = sets(ppts,0,any_value);
    sets(ppts,index,any_where);
    m_affected_count =  ppts->update();
    return *this;
}
void newobj::sql::update::clear()
{
    m_affected_count = 0;
    m_fields.clear();
    m_table.clear();
    m_limit.start = 0;
    m_limit.count = 0;
    m_values.clear();
    m_wheres.clear();

}



newobj::sql::query::query()
{

}
newobj::sql::query::~query()
{

}
newobj::sql::query& newobj::sql::query::table(const nstring& table)
{
    this->m_table = table;
    return *this;
}
newobj::sql::query& newobj::sql::query::field(const std::initializer_list<nstring>& fields)
{

    this->m_fields = fields;
    return *this;
}
newobj::sql::query& newobj::sql::query::where(const std::initializer_list<sql::where>& wheres)
{

    this->m_wheres = wheres;
    return *this;
}
newobj::sql::query& newobj::sql::query::limit(uint64 start,uint64 len)
{
    this->m_limit.start = start;
    this->m_limit.count = len;
    return *this;
}
newobj::sql::query& newobj::sql::query::orderby(const std::initializer_list<sql::orderby>& bys)
{
    m_orderbys = bys;
    return *this;
}
newobj::sql::query& newobj::sql::query::exec(newobj::mysql_plus::conn* conn)
{
    nstring sql(1024);
    sql.append("SELECT ");
    for(size_t i=0;i<m_fields.size();i++)
    {
        if(i+1 == m_fields.size())
            sql.append(m_fields[i]);
        else
            sql.append(m_fields[i]+",");
    }
    sql.append(" FROM ");
    sql.append(m_table);
    if(m_orderbys.size() != 0){
        sql.append(" ORDER BY ");
        for(size_t i=0;i<m_orderbys.size();i++)
        {
            sql.append(" "+m_orderbys[i].name+" "+(m_orderbys[i].type == sql::orderby::ASC?"ASC":"DESC"));
            if(i+1 != m_orderbys.size())
                sql.append(",");
        }
    }
    make_where(sql,m_wheres);
    if(m_limit.count != 0)
        sql.append(" LIMIT "+nstring::from(m_limit.start)+","+nstring::from(m_limit.count));

    std::vector<std::any> any_where;
    for(size_t i=0;i<m_wheres.size();i++)
    {
        if(m_wheres[i].value.type() == typeid(void))
            continue;
        any_where.push_back(m_wheres[i].value);
    }
    auto ppts = conn->setsql(sql);
    sets(ppts,1,any_where);
    m_result = ppts->query();
    return *this;
}
newobj::mysql_plus::result* newobj::sql::query::result()
{
    return m_result;
}
void newobj::sql::query::clear()
{
    m_fields.clear();
    m_table.clear();
    m_limit.start = 0;
    m_limit.count = 0;
    m_wheres.clear();
    m_orderbys.clear();
    m_result = nullptr;
}






newobj::sql::del::del()
{

}
newobj::sql::del::~del()
{

}
newobj::sql::del& newobj::sql::del::table(const nstring& table)
{
    this->m_table = table;
    return *this;
}
newobj::sql::del& newobj::sql::del::where(const std::initializer_list<sql::where>& wheres)
{

    this->m_wheres = wheres;
    return *this;
}
newobj::sql::del& newobj::sql::del::limit(uint64 start,uint64 len)
{
    this->m_limit.start = start;
    this->m_limit.count = len;
    return *this;
}
newobj::sql::del& newobj::sql::del::exec(newobj::mysql_plus::conn* conn)
{
    nstring sql(1024);
    sql.append("DELETE FROM ");
    sql.append(m_table);
    make_where(sql,m_wheres);
    if(m_limit.count != 0)
        sql.append(" LIMIT "+nstring::from(m_limit.start)+","+nstring::from(m_limit.count));

    std::vector<std::any> any_where;
    for(size_t i=0;i<m_wheres.size();i++)
    {
        if(m_wheres[i].value.type() == typeid(void))
            continue;
        any_where.push_back(m_wheres[i].value);
    }
    auto ppts = conn->setsql(sql);
    sets(ppts,1,any_where);
    m_affected_count =  ppts->update();
    return *this;
}
void newobj::sql::del::clear()
{
    m_affected_count = 0;
    m_fields.clear();
    m_table.clear();
    m_limit.start = 0;
    m_limit.count = 0;
    m_wheres.clear();

}


#endif