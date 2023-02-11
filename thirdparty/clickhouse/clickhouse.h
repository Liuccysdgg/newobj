#pragma once

#include "public/define.h"
#include <string>
#include "util/forstring.h"
#include <vector>
#include "util/json.h"
#include "public/base.h"
#include "public/buffer.h"
#include "public/string.h"
#include "util/lock.h"

#include "util/queue.hpp"
#include "util/pool.hpp"
#include "clickhouse/client.h"

namespace newobj
{
    namespace clickhouse
    {
         template<typename T,typename Z>
         inline Z nullable_to(const ::clickhouse::Block& block,int field_index,size_t count_index){
            auto value = block[field_index]->As<::clickhouse::ColumnNullable>();
            if (value->IsNull(count_index) == false) {
                return value->Nested()->As<T>()->At(count_index);
            }
            return Z();
         } 
        struct conn_info
        {
            conn_info()
            {
                port = 0;
                size = 0;
            }
            nstring ipaddress;
            nstring password;
            nstring database;
            nstring username;
            uint32 port;
            uint32 size;
        };
        class NEWOBJ_API conn :public newobj::example<newobj::clickhouse::conn_info>, public newobj::base
        {
        public:
            conn();
            ~conn();
            virtual bool start(const newobj::clickhouse::conn_info& info) override;
            virtual void close() override;
            virtual void recover() override;
            virtual void task_out() override;
            ::clickhouse::Client* get();
            void update(const ::clickhouse::Query& query);
        private:
            void* m_handle;
            ::clickhouse::ClientOptions m_option;
            ::clickhouse::Client* m_client;
        };
        class NEWOBJ_API pool : public newobj::pool<newobj::clickhouse::conn, newobj::clickhouse::conn_info>
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


//			class Tran;
//			enum RDStat
//			{
//				//成功
//				RD_OK = 1,
//				//失败
//				RD_ERROR = -1,
//				//已存在
//				RD_EXISTED = 2,
//				//不存在
//				RD_NOEXIST = 3,
//				//无任何操作
//				RD_NOMOVE = 4
//			};
//			typedef void(*CALLBACK_CONN_OVER_RD)(uint32 num);
//			class NEWOBJ_API Pool
//			{
//			public:
//				Pool();
//				~Pool();
//				void Recover(ptr conn);
//				bool Init(const nstring& ipaddress, int port, const nstring& password, int size, bool create_new, int timout_connect_sec = 3, int timeout_io_sec = 5);
//				ptr Get(bool create_new = true);
//			private:
//				ptr m_acl_redis_pool;
//				newobj::mutex m_mutex;
				
//			public:
//				bool m_createNew;
//				/*设置连接执行结束，回调执行次数*/
//				void SetOverCB(CALLBACK_CONN_OVER_RD cb);
//				CALLBACK_CONN_OVER_RD m_connOverCb;
//			};
//			class NEWOBJ_API Conn:public newobj::base
//			{
//			public:
//				Conn(ptr pool);
//				~Conn();
//                void keys(const nstring &pam,std::vector<nstring> &_keys);
//                void keysvalue(const nstring &pam,std::vector<nstring> &_keys,std::vector<nstring>&values);
//				RDStat setnx(const nstring& key, const nstring& value);
//				void set(const nstring& key, const nstring& value);
//				void set(const char* key, const char* value,at_int key_len,at_int value_len);
//				/*flat参数参考宏 SETFLAG_*/
//				void set_timeout(const nstring& key, const nstring& value, uint32 timeout, int flag);
//				bool get(const nstring& key, nstring& value);
//                bool get(const nstring& key, newobj::json &json);
//				/*
//				* @Return
//				*		RD_OK、RD_NOEXIST
//				*/
//				RDStat expire(const nstring& key, uint32 timeout_sec);
//				/*
//				* @Return
//				*		RD_OK、RD_NOMOVE
//				*/
//				RDStat del(const nstring& key);
//				bool exist(const nstring& key);



//				RDStat hsetnx(const nstring& key, const nstring& name, const nstring& value, uint32 timeout_sec = -1);
//				/*return: 0=新增成功 1=更新成功*/
//				uint32 hset(const nstring& key,const nstring& name,const nstring& value,uint32 timeout_sec = -1);
//				uint32 hset(const nstring& key, const nstring& name,uint64 value, uint32 timeout_sec = -1);
//				void hmset(const nstring& key,const std::vector<Kv> &kv, uint32 timeout_sec = -1);

//				void hmget(const nstring& key, const std::vector<nstring>& names,std::vector<nstring>&values);
//                bool hgetall (const nstring& key, std::vector<nstring>& names, std::vector<nstring>& values);
				
//				void hdel(const nstring& key,uint64 name);
//				void hdel(const nstring& key, const nstring& name);
//				bool hget(const nstring& key, const nstring& name,uint32& value);
//				bool hget(const nstring& key, const nstring &name,nstring& value);
//                bool hget(const nstring& key, const nstring &name,newobj::json &jsonValue);
//				bool hexist(const nstring& key, const nstring& name);
//				bool hkeys(const nstring& key, std::vector<nstring>& names);
				
//				void tran(Tran& tran);
//				/*列表加入到头部*/
//				void lpush(const nstring&key,const nstring &value);
//				/*列表取出尾部*/
//				bool rpop(const nstring &key,nstring &value);
//			private:
//				void GetConn();
//			private:
//				ptr m_pool;
//				ptr m_conn;
//				uint32 m_run_num;	//执行次数
//			};
//			class NEWOBJ_API Tran
//			{
//			public:
//				Tran();
//				~Tran();
//				void watch(const nstring& key);
//				void watch(const std::vector<nstring>& keys);
//				void unwatch();
//				void multi();
//				bool exec();
//				void exec_trw();
//				void discard();
//				void cmd(const nstring& cmd, const std::vector<nstring>& param);
//				Conn* GetConn();
//				friend class Conn;
//			private:
//				acl::redis_transaction* m_tran;
//				Conn* m_conn;
//			};
//	}
//}
