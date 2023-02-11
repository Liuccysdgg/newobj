#pragma once
#if 0

#include "util/forstring.h"
#include "public/var.h"
#include "util/json.h"
#include "tp_sqlstatement.h"

#ifdef LIB_DB_MYSQL
#ifndef MSVC_2010
#include <initializer_list>
namespace newobj
{
	namespace web
	{
		/*分页查询参数*/
		struct pageing_query_param
		{
			//请求json参数名
			nstring query_name;
			//解析类型
			NVarType type;
			//最后生成SQL查询的字段名，默认则和 queryName 一致
			nstring sql_field;
		};


		/***********************************************************
		 * Class：分页模板
		 * Desc：前端分页请求
		 ***********************************************************/
		class NEWOBJ_API pageing
		{
		public:
			pageing(newobj::network::http::controller* httpCTL,newobj::db::mysql::pool* pool);
			~pageing();
			pageing& fields(std::initializer_list<nstring> fields);
			pageing& table(const nstring& table);
			/*最大单页数量*/
			pageing& max_count(uint32 pageMaxCount);
			pageing& where(const newobj::where& where);
			pageing& orderby(const nstring& orderBy, newobj::sort_type sort);
			/*取查询数量*/
			uint64 count();
			/*取查询结果*/
			void query(newobj::json& tableJson);
			nvar param(const nstring& name, NVarType varType);
			friend class newobj::network::http::controller;
		private:
			//查询条件
			newobj::where2 m_where;
			newobj::network::http::controller* m_httpCtl;
			//最大单页数量
			uint32 m_pageMaxCount = 10;
			//查询字段
			std::vector<nstring> m_fields;
			//表
			nstring m_table;
			//排序
			newobj::sort m_sort;
			// 连接池
			db::mysql::pool* m_pool;
		};
	}

}


#endif
#endif
#endif
