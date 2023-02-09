#pragma once


#include "util/thread.h"
#include "public/define.h"
#include <vector>
#include <string>
#include <list>
#include "public/base.h"
#include "util/lock.h"
#include "util/queue.hpp"
#include "util/point.hpp" 
#include "util/pool.hpp"
#ifndef _WIN32
#include <stddef.h>
#endif

namespace newobj
{
	namespace mysql
	{

		class pool;
		class result;
		class conn;
        struct mysql_conn_info
        {
            mysql_conn_info(){
                port = 0;
            }
            nstring ipaddress;
            nstring username;
            nstring password;
            nstring database;
            uint32 port;
        };

		/*事务*/
		class NEWOBJ_API transaction :public newobj::base
		{
		public:
			transaction();
			~transaction();
			void begin();
			void commit();
			void rollback();
			friend class conn; 
		private:
			class conn* m_conn;
			// 已经开启事务
			bool m_begin;
		};
        class NEWOBJ_API conn :public newobj::example<newobj::mysql::mysql_conn_info>,public newobj::base
		{
		public:
			conn();
			~conn();
            virtual bool start(const newobj::mysql::mysql_conn_info& info) override;
            virtual void close() override;
            virtual void recover() override;
			void query(const nstring& sql,result& result);
			uint64 update(const nstring& sql);
			void transaction(transaction& tran);
			uint64 insert_id();

			nstring error();

			friend class newobj::mysql::pool;
			friend class newobj::mysql::transaction;
		private:
			void* m_handle;
            newobj::mysql::mysql_conn_info m_info;
			class transaction* m_transaction;
		};
		class NEWOBJ_API result :public newobj::base
		{
		public:
			result(); 
			~result();
			// 列数量
			uint32 field_count();
			// 行数量
			uint64 row_count();
			// 取文本值
			nstring get(uint32 field);
			// 下一行
			bool next();
			friend class newobj::mysql::conn;
		private:
			void* m_res;
			uint32 m_field_count;
			uint32 m_row_count;
			// 当前行数据
			char** m_row;
		};
        class NEWOBJ_API pool: public newobj::pool<newobj::mysql::conn,newobj::mysql::mysql_conn_info>
		{
		public:
            pool()
            {

            }
            ~pool()
            {

            }
		};
	}
}
