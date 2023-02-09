#pragma once
#include <string>
#include "public/define.h"
#include <initializer_list>
#include "public/var.h"
#include "public/string.h"
#include "util/json.h"
#ifndef MSVC_2010
#ifdef LIB_DB_MYSQL
namespace newobj
{
	namespace db
	{
		namespace mysql
		{
			class conn;
			class result;
			class prepared_statement;
		}
	}
		class NEWOBJ_API isqlstatement
		{
		public:
			isqlstatement(newobj::db::mysql::pool* pool);
			virtual void clear() = 0;
		protected:
			std::shared_ptr<db::mysql::conn> m_conn;
		};
		enum sort_type
		{
			DESC,
			ASC
		};
		struct sort
		{
			nstring field;
			sort_type type;
		};
		/*where查询条件*/
		struct where
		{
			//填充值
			nstring sql;
			std::initializer_list<nvar> fill;
		};
		struct where2
		{
			//填充值
			nstring sql;
			std::vector<nvar> fill;
		};
		static inline void sets(newobj::db::mysql::prepared_statement* pptt, const std::vector<newobj::nvar>& m_values, int32 start = 1);
		class NEWOBJ_API insert :public isqlstatement
		{
		public:
			insert(newobj::db::mysql::pool* pool);
			~insert();
			insert& table(const nstring& table);
			insert& field(const std::initializer_list<nstring>& fields);
			insert& value(const std::initializer_list<newobj::nvar>& values);
			bool exec();
			// 通过 ISqlStatement 继承
			virtual void clear() override;
			uint64 last_insert_id();
		private:
			nstring m_table;
			std::vector<nstring> m_fields;
			std::vector<newobj::nvar> m_values;
		};
		class NEWOBJ_API update :public isqlstatement
		{
		public:
			update(newobj::db::mysql::pool* pool);
			~update();
			update& table(const nstring& table);
			update& field(const std::initializer_list<nstring>& fields);
			/*相对于Field来说，该方法不需要Value配合设置?的值*/
			update& set(const std::initializer_list<nstring>& sets);
			update& value(const std::initializer_list<newobj::nvar>& values);
			update& where(const newobj::where& where);
			update& limit(uint32 start, int32 len = -2);
			uint64 exec();
			// 通过 ISqlStatement 继承
			virtual void clear() override;
		private:
			nstring m_table;
			std::vector<nstring> m_fields;
			std::vector<nstring> m_sets;

			std::vector<newobj::nvar> m_values;
			newobj::where m_where;
			nstring m_limit;


		};
		class NEWOBJ_API query :public isqlstatement
		{
		public:
			query(newobj::db::mysql::pool* pool);
			~query();
			query& table(const nstring& table);
			//Query& Field(const std::initializer_list<nstring>& fields);
			query& field(const std::vector<nstring>& fields);
			//Query& Where(const Where& where);
			query& where(const where2& where);
			query& limit(uint64 start, int len = -2);
			query& orderby(const nstring& orderBy, sort_type sort);
			uint64 exec();
			bool next();
			uint64 row();

			nstring to_string(uint32 index);
			int64 to_int64(uint32 index);
			int32 to_int32(uint32 index);
			uint64 to_uint64(uint32 index);
			uint32 to_uint32(uint32 index);
			double to_double(uint32 index);
			newobj::json to_json(uint32 index);


			nstring to_string(const nstring& name);
			int64 to_int64(const nstring& name);
			int32 to_int32(const nstring& name);

			uint64 to_uint64(const nstring& name);
			uint32 to_uint32(const nstring& name);
			double to_double(const nstring& name);
			newobj::json to_json(const nstring& name);

			void first();
			void to_json_array(newobj::json& jsonArray);
			void to_json_line(newobj::json& jsonObj);

			uint32 column();
			nstring column_name(uint32 index);

			// 通过 ISqlStatement 继承
			virtual void clear() override;
		private:
			nstring m_table;
			std::vector<nstring> m_fields;
			newobj::where2 m_where;
			nstring m_limit;
			newobj::sort m_sort;
			
			db::mysql::prepared_statement* m_pptt;
			db::mysql::result* m_result;
		};
		class NEWOBJ_API del:private isqlstatement
		{
		public:
			del(newobj::db::mysql::pool* pool);
			~del();
			del& table(const nstring& table);
			del& where(const newobj::where& where);
			del& limit(uint32 start, int32 len = -2);
			uint64 exec();
		private:
			nstring m_table;
			newobj::where m_where;
			nstring m_limit;

			// 通过 isqlstatement 继承
			virtual void clear() override;
		};
	}
#endif
#endif