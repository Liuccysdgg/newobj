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
    namespace mysql_plus
    {


        class pool;
        class result;
        class conn;
        class prepare_statement;
        struct mysql_conn_info
        {
            mysql_conn_info(){
                port = 0;
            }
            nstring ipaddress;
            nstring username;
            nstring password;
            nstring database;
            nstring charset;
            uint32 port;
        };
        class NEWOBJ_API result :public newobj::base
        {
        public:
            result();
            ~result();
            // 列数量
            uint32 field_count();
            // 行数量
            size_t row_count();
            // 取文本值
            nstring get(uint32 field);
            // 下一行
            bool next();
            int32 get_int32(uint32 index);
            int32 get_int32(const nstring& name);

            uint32 get_uint32(uint32 index);
            uint32 get_uint32(const nstring& name);

            int64 get_int64(uint32 index);
            int64 get_int64(const nstring& name);

            uint64 get_uint64(uint32 index);
            uint64 get_uint64(const nstring& name);

            nstring get_string(uint32 index);
            nstring get_string(const nstring& name);

            bool get_boolean(uint32 index);
            bool get_boolean(const nstring& name);

            double get_double(uint32 index);
            double get_double(const nstring& name);



            friend class newobj::mysql_plus::prepare_statement;
        private:
            void* m_handle;
            uint32 m_field_count;
            uint32 m_row_count;
        };

        class NEWOBJ_API prepare_statement:public newobj::base{
        public:
            prepare_statement();
            ~prepare_statement();
            void set_bigint(uint32 index,const nstring& value);
            void set_boolean(uint32 index,bool value);
            void set_datetime(uint32 index,const nstring& value);
            void set_double(uint32 index,double value);
            void set_int32(uint32 index,int32 value);
            void set_uint32(uint32 index,uint32 value);
            void set_int64(uint32 index,int64 value);
            void set_uint64(uint32 index,uint64 value);
            void set_null(uint32 index);
            void set_string(uint32 index,const nstring& value);
            void clear();
            uint64 update();
            newobj::mysql_plus::result* query();

            friend class newobj::mysql_plus::conn;
        private:
            newobj::mysql_plus::result* m_result;
            void* m_handle;
        };
        class NEWOBJ_API conn :public newobj::example<newobj::mysql_plus::mysql_conn_info>,public newobj::base
        {
        public:
            conn();
            ~conn();
            virtual bool start(const newobj::mysql_plus::mysql_conn_info &info) override;
            virtual void close() override;


            virtual void recover() override;
            virtual void task_out() override;


            void clear();
            newobj::mysql_plus::prepare_statement* setsql(const nstring& sql);
            uint64 insert_id();

            void begin(bool autocommit = false);
            void commit();
            void rollback();

            friend class newobj::mysql_plus::pool;
        private:
            void *m_handle;
            newobj::mysql_plus::mysql_conn_info m_info;
            class prepare_statement* m_ppst;
        };


        class NEWOBJ_API pool: public newobj::pool<newobj::mysql_plus::conn,newobj::mysql_plus::mysql_conn_info>
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
