#pragma once
#include "public/define.h"
#include <vector>
#include "util/queue.hpp"
#include "util/json.h"
#if 0
#ifndef MSVC_2010
namespace newobj
{
	namespace db
	{
		namespace redis
		{
			class Pool;
		}
		namespace mysql
		{
			class Pool;
		}
	}
	class node;
	/**************************************************************
	 * Class��Node�ڵ㻺����
	 * Desc����������������Nodeָ��
	 **************************************************************/
	class node_cache
	{
	public:

		node_cache(newobj::db::redis::Pool* redisPool, newobj::db::mysql::Pool* sqlPool);
		~node_cache();
		/*********************************************************
		 * Fun����ȡ
		 *********************************************************/
		newobj::node* get();
		/*********************************************************
		 * Fun������
		 * Param��
		 *			node		:		�ڵ�ָ��
		 *********************************************************/
		void recover(node* node);
		friend class Node;
	private:
		/*********************************************************
		 * Fun������
		 *********************************************************/
		node* create();
	private:
		//Nodeָ�����
		queue<node*> m_queue;
		//����������
		const uint32 m_max = 100;
		//RedisPool
		newobj::db::redis::Pool* m_redis_pool = nullptr;
		//MysqlPool
		newobj::db::mysql::Pool* m_mysql_pool = nullptr;
	};
	/**************************************************************
	 * Class��Node�ڵ�
	 * Ver��   1.0
	 * Desc��
	 *			1.0�����redis��mysql���洦��ڵ�
	 **************************************************************/
	class node
	{
	public:
		node(node_cache* nodeCache);
		~node();
		/*********************************************************
		 * Fun��������
		 *********************************************************/
		void operator=(const nstring& name);
		/*********************************************************
		 * Fun��������
		 *********************************************************/
		void operator>>(const nstring& value);


		void set(const nstring& value);
		/*********************************************************
		 * Fun��������
		 * Param��
		 *			value		:		����
		 *			timeout	:		��ʱʱ�䣬������룬�������flag����
		 *			flag			:		д����������գ�SETFLAG_ ��
		 *********************************************************/
		void set(const nstring& value, int32 timeout, int32 flag = SETFLAG_EX);
		/*********************************************************
		  * Fun��������
		  * Param��
		  *			value		:		����
		  ********************************************************/
		newobj::db::redis::RDStat set_nx(const nstring& value);
		/*********************************************************
		 * Fun��ȡ�ӽڵ�
		 * Param��
		 *			name		��		�ӽڵ�����
		 *********************************************************/
		newobj::node* next(const nstring& name);
		/*********************************************************
		 * Fun��ȡֵ
		 *********************************************************/
		nstring to_string();
		uint32 to_uint32();
		int32 to_int32();
		uint64 to_uint64();
		int64 to_int64();
		void to_json(json& value);
		/*********************************************************
		 * Fun��ɾ��
		 *********************************************************/
		void del();
		/*********************************************************
		 * Fun�����ù���ʱ��
		 * Desc�����ù���ʱ���Ĭ��ɾ���־ô���
		 *********************************************************/
		void expire(int32 timeout_sec);
		/*********************************************************
		 * Fun��[hash] ȡֵ
		 * Param��
		 *			name		:		hash��
		 *********************************************************/
		const nstring operator[](const nstring& name);
		/*********************************************************
		 * Fun��[hash] ��ֵ
		 * Param��
		 *			name				:		hash��
		 *			value				��		hashֵ
		 *********************************************************/
		void hset(const nstring& name, const nstring& value);
		/*********************************************************
		 * Fun��[hash] ��ֵ
		 * Param��
		 *			name				:		hash��
		 *			value				��		hashֵ
		 *********************************************************/
		newobj::db::redis::RDStat hset_nx(const nstring& name, const nstring& value);
		newobj::db::redis::RDStat hset_nx(const uint64& name, const nstring& value);
		newobj::db::redis::RDStat hset_nx(const nstring& name, const json& value);
		newobj::db::redis::RDStat hset_nx(const uint64& name, const json& value);
		/*********************************************************
		 * Fun��[hash] ȡֵ
		 * Param��
		 *			name				:		hash��
		 *			value				��		hashֵ
		 *********************************************************/
		bool hget(const nstring& name, nstring& value);
		bool hget(const nstring& name, json& value);
		bool hget(const uint64& name, nstring& value);
		bool hget(const uint64& name, json& value);
		/*********************************************************
		 * Fun��[hash] �Ƿ����
		 *********************************************************/
		 /*void Exist();*/
		friend class NodeCache;
	private:
		void update_sql(const nstring& value);
		bool query_sql(nstring& value);
	private:
		//����
		nstring m_name;
		//����ָ��
		node* m_next_node_ptr = nullptr;
		//�����
		node_cache* m_nodeCache = nullptr;
		//ShaName
		nstring m_sha1_name;
		//��ͷ1�ֽ�
		nstring m_pre_1;
	};

}
#endif
#endif
