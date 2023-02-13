#pragma once
#include <string>
#include "public/define.h"
#include <initializer_list>
#include "public/string.h"
#include "util/json.h"
#include "mysql/mysql_plus.h"
#include <any>
namespace newobj
{
    namespace sql{

        		/*where查询条件*/
		struct NEWOBJ_API where
		{
            // 字段名称
            nstring name;
            // 比较符号
            nstring code;
            // 值
            std::any value;
		};
        // 分页
        struct NEWOBJ_API limit{
            uint64 start = 0;
            uint64 count = 0;
        };
        // 更新值
        struct NEWOBJ_API value{
            // 名称
            nstring name;
            // 值
            std::any val;
        };
        // 排序
        struct NEWOBJ_API orderby{
            // 排序类型
            enum sort_type
		    {
			    DESC,
			    ASC
		    };
            nstring name;
            sort_type type;
        };
        class NEWOBJ_API isql
        {
        public:
            virtual void clear() = 0;
        };
        class NEWOBJ_API insert:public isql
		{
		public:
			insert();
			~insert();
			newobj::sql::insert& table(const nstring& table);
			newobj::sql::insert& field(const std::initializer_list<nstring>& fields);
			newobj::sql::insert& value(const std::initializer_list<std::any>& values);
            newobj::sql::insert& exec(newobj::mysql_plus::conn* conn);
            inline uint64 affected_count(){
                return m_affected_count;
            }
            virtual void clear() override;
		private:
			nstring m_table;
            std::vector<nstring> m_fields;
            std::vector<std::vector<std::any>> m_values;
            uint64 m_affected_count = 0;
		};

        class NEWOBJ_API update :public isql
		{
		public:
			update();
			~update();
			update& table(const nstring& table);
			update& field(const std::initializer_list<nstring>& fields);
			update& where(const std::initializer_list<sql::where>& wheres);
			update& set(const std::initializer_list<sql::value>& values);
			update& limit(uint64 start,uint64 len);
			update& exec(newobj::mysql_plus::conn* conn);
            inline uint64 affected_count(){
                return m_affected_count;
            }
			virtual void clear() override;
		private:
			nstring m_table;
			std::vector<nstring> m_fields;
            uint64 m_affected_count = 0;
			std::vector<sql::where> m_wheres;
            std::vector<sql::value> m_values;
            sql::limit m_limit;

		};
        class NEWOBJ_API query :public isql
        {
        public:
            query();
            ~query();
            query& table(const nstring& table);
            query& field(const std::initializer_list<nstring>& fields);
			query& where(const std::initializer_list<sql::where>& wheres);
            query& limit(uint64 start,uint64 len);
            query& orderby(const std::initializer_list<sql::orderby>& bys);
            query& exec(newobj::mysql_plus::conn* conn);
            mysql_plus::result* result();
            virtual void clear() override;
        private:
            nstring m_table;
            std::vector<nstring> m_fields;
			std::vector<sql::where> m_wheres;
            sql::limit m_limit;
            std::vector<sql::orderby> m_orderbys;
            mysql_plus::result *m_result = nullptr;
        };
        class NEWOBJ_API del :public isql
		{
		public:
			del();
			~del();
			del& table(const nstring& table);
			del& where(const std::initializer_list<sql::where>& wheres);
			del& limit(uint64 start,uint64 len);
			del& exec(newobj::mysql_plus::conn* conn);
            inline uint64 affected_count(){
                return m_affected_count;
            }
			virtual void clear() override;
		private:
			nstring m_table;
			std::vector<nstring> m_fields;
            uint64 m_affected_count = 0;
			std::vector<sql::where> m_wheres;
            sql::limit m_limit;
		};

    }
}
