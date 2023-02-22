#pragma once

#include "public/define.h"
#include <string>
#include "util/forstring.h"
#include <vector>
#include "util/json.h"
#include "public/base.h"
#include "public/buffer.h"
#include "basetype/string.h"
#include "util/lock.h"

#include "util/queue.hpp"
#include "util/pool.hpp"
#ifndef _WIN32
#include "hiredis/hiredis.h"
#endif
namespace newobj
{
    namespace redis
    {
        struct conn_info
        {
            conn_info()
            {
                port = 0;
                size = 0;
            }
            nstring ipaddress;
            nstring password;
            uint32 port;
            uint32 size;
        };
#ifndef _WIN32
        class NEWOBJ_API conn :public newobj::example<newobj::redis::conn_info>, public newobj::base
        {
        public:
            conn();
            ~conn();
            virtual bool start(const newobj::redis::conn_info& info) override;
            virtual void close() override;
            virtual void recover() override;
            virtual void task_out() override;
            void setnx(const nstring& key, const nstring& value);
            void setxx(const nstring& key, const nstring& value);
            void setex(const nstring& key, const nstring& value, int64 timeout_sec);
            void set(const nstring& key, const nstring& value);
            void expire(const nstring& key, int64 timeout_sec);
            bool get(const nstring& key, int64& value);
            bool get(const nstring& key, nstring& value);
            bool get(const nstring& key, newobj::json& json);
            void del(const nstring& key);
            bool exists(const nstring& key);
            bool ttl(const nstring& key, timestamp& sec);
            bool pttl(const nstring& key, timestamp& msec);

            bool hdel(const nstring& key, const nstring& field);
            bool hexists(const nstring& key, const nstring& field);
            bool hget(const nstring& key, const nstring& field, nstring& value);
            void hset(const nstring& key, const nstring& field, const nstring& value);


            /*ִ�д���*/
            nstring udpate(const nstring& cmd);
        private:
            /*У��redisReply�Ƿ�ɹ�,�����򷵻ش�����־*/
            nstring check_reply(void* reply);

            /*��ѯ*/
            nstring query(const nstring& cmd, redisReply*& reply);
            nstring query_int(const nstring& cmd, bool& exist, int64& value);
            nstring query_string(const nstring& cmd, bool& exist, nstring& value);

            void free_reply_object(void* reply);
        private:
            void* m_handle;
            newobj::redis::conn_info m_info;
        };
        class NEWOBJ_API pool : public newobj::pool<newobj::redis::conn, newobj::redis::conn_info>
        {
        public:
            pool()
            {

            }
            ~pool()
            {

            }
        };
#endif
    }
}


//			class Tran;
//			enum RDStat
//			{
//				//�ɹ�
//				RD_OK = 1,
//				//ʧ��
//				RD_ERROR = -1,
//				//�Ѵ���
//				RD_EXISTED = 2,
//				//������
//				RD_NOEXIST = 3,
//				//���κβ���
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
//				/*��������ִ�н������ص�ִ�д���*/
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
//				/*flat�����ο��� SETFLAG_*/
//				void set_timeout(const nstring& key, const nstring& value, uint32 timeout, int flag);
//				bool get(const nstring& key, nstring& value);
//                bool get(const nstring& key, newobj::json &json);
//				/*
//				* @Return
//				*		RD_OK��RD_NOEXIST
//				*/
//				RDStat expire(const nstring& key, uint32 timeout_sec);
//				/*
//				* @Return
//				*		RD_OK��RD_NOMOVE
//				*/
//				RDStat del(const nstring& key);
//				bool exist(const nstring& key);



//				RDStat hsetnx(const nstring& key, const nstring& name, const nstring& value, uint32 timeout_sec = -1);
//				/*return: 0=�����ɹ� 1=���³ɹ�*/
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
//				/*�б���뵽ͷ��*/
//				void lpush(const nstring&key,const nstring &value);
//				/*�б�ȡ��β��*/
//				bool rpop(const nstring &key,nstring &value);
//			private:
//				void GetConn();
//			private:
//				ptr m_pool;
//				ptr m_conn;
//				uint32 m_run_num;	//ִ�д���
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
