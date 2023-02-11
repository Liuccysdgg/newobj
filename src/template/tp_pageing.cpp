#if 0
#include "tp_pageing.h"
#include "public/exception.h"
#ifndef MSVC_2010
#ifdef LIB_DB_MYSQL
newobj::web::pageing::pageing(newobj::network::http::controller* httpCTL, newobj::db::mysql::pool* pool)
{
	this->m_pool = pool;
	this->m_httpCtl = httpCTL;
}
newobj::web::pageing::~pageing()
{
}

newobj::web::pageing& web::pageing::fields(std::initializer_list<nstring> fields)
{
	// TODO: 在此处插入 return 语句
	this->m_fields = fields;
	return *this;
}

newobj::web::pageing& web::pageing::table(const nstring& table)
{
	// TODO: 在此处插入 return 语句
	this->m_table = table;
	return *this;
}

newobj::web::pageing& web::pageing::max_count(uint32 pageMaxCount)
{
	// TODO: 在此处插入 return 语句
	this->m_pageMaxCount = pageMaxCount;
	return *this;
}

newobj::web::pageing& web::pageing::where(const newobj::where& where)
{
	// TODO: 在此处插入 return 语句
	this->m_where.fill = where.fill;
	this->m_where.sql = where.sql;
	return *this;
}

newobj::web::pageing& web::pageing::orderby(const nstring& orderBy, newobj::sort_type sort)
{
	// TODO: 在此处插入 return 语句
	this->m_sort.field = orderBy;
	this->m_sort.type = sort;
	return *this;
}

uint64 newobj::web::pageing::count()
{
	newobj::query query(this->m_pool);
	query
		.field({ "COUNT(*)" })
		.table(this->m_table)
		.where(this->m_where)
		.exec();
	query.next();
	return query.to_int64(1);
}
void newobj::web::pageing::query(newobj::json& tableJson)
{
	//分页页码及条数
	if (((network::http::request*)m_httpCtl)->to_json()["query"]["page"].is_empty())
	{
		trw_str("Template [Pageing]: missing 'query->page' JSON object in request parameter");
	}
	//uint64 start = 0;
	//uint32 line = 0;
	auto start = ((network::http::request*)m_httpCtl)->to_json()["query"]["page"]["start"];
	auto line = ((network::http::request*)m_httpCtl)->to_json()["query"]["page"]["line"];
	if (!start.is_number())
	{
		trw_str("Template [Pageing]: missing 'query->page[\"start\"]' JSON object in request parameter");
	}
	if (!line.is_number())
	{
		trw_str("Template [Pageing]: missing 'query->page[\"line\"]' JSON object in request parameter");
	}

	if (line.to<uint32>() > m_pageMaxCount)
	{
		trw_str("Template [Pageing]: Maximum page rows exceeded");
	}

	newobj::query query(this->m_pool);
	query
		.field(this->m_fields)
		.table(this->m_table)
		.where(this->m_where)
		.limit(start.to<uint32>(), line.to<uint32>())
		.orderby(this->m_sort.field,this->m_sort.type)
		.exec();
	query.to_json_array(tableJson);

}

nvar newobj::web::pageing::param(const nstring& name, NVarType varType)
{
	// TODO: 在此处插入 return 语句
	newobj::nvar var;
	bool find = false;
	auto qryf = ((network::http::request*)m_httpCtl)->to_json()["query"][name];
	switch (varType)
	{
	case NVarType::NVT_Int:
		var = ((network::http::request*)m_httpCtl)->to_json()["query"][name].to<int32>();
		break;
	case NVarType::NVT_UInt:
		var = ((network::http::request*)m_httpCtl)->to_json()["query"][name].to<uint32>();
		break;
	case NVarType::NVT_LLong:
		var = ((network::http::request*)m_httpCtl)->to_json()["query"][name].to<int64>();
		break;
	case NVarType::NVT_ULLong:
		var = ((network::http::request*)m_httpCtl)->to_json()["query"][name].to<uint64>();
		break;
	case	NVarType::NVT_String:
		var = ((network::http::request*)m_httpCtl)->to_json()["query"][name].to<nstring>();
		break;
	case NVarType::NVT_Float:
	case NVarType::NVT_Double:
		var = ((network::http::request*)m_httpCtl)->to_json()["query"][name].to<double>();
		break;
	default:
		trw_str("Template [Pageing]: Other variable types are not recognized");
		break;
	}
	if (find == false)
	{
		return NVT_Null;
		//trw_str("Template [Pageing]: Request parameter '" + name + "' not found");
	}
	return var;
}

#endif
#endif
#endif
