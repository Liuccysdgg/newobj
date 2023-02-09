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
	 * Class：Node节点缓存器
	 * Desc：用于生产及管理Node指针
	 **************************************************************/
	class node_cache
	{
	public:

		node_cache(newobj::db::redis::Pool* redisPool, newobj::db::mysql::Pool* sqlPool);
		~node_cache();
		/*********************************************************
		 * Fun：获取
		 *********************************************************/
		newobj::node* get();
		/*********************************************************
		 * Fun：回收
		 * Param：
		 *			node		:		节点指针
		 *********************************************************/
		void recover(node* node);
		friend class Node;
	private:
		/*********************************************************
		 * Fun：创建
		 *********************************************************/
		node* create();
	private:
		//Node指针队列
		queue<node*> m_queue;
		//最大队列数量
		const uint32 m_max = 100;
		//RedisPool
		newobj::db::redis::Pool* m_redis_pool = nullptr;
		//MysqlPool
		newobj::db::mysql::Pool* m_mysql_pool = nullptr;
	};
	/**************************************************************
	 * Class：Node节点
	 * Ver：   1.0
	 * Desc：
	 *			1.0：针对redis及mysql缓存处理节点
	 **************************************************************/
	class node
	{
	public:
		node(node_cache* nodeCache);
		~node();
		/*********************************************************
		 * Fun：置名称
		 *********************************************************/
		void operator=(const nstring& name);
		/*********************************************************
		 * Fun：置内容
		 *********************************************************/
		void operator>>(const nstring& value);


		void set(const nstring& value);
		/*********************************************************
		 * Fun：置内容
		 * Param：
		 *			value		:		内容
		 *			timeout	:		超时时间，毫秒或秒，具体参照flag参数
		 *			flag			:		写入参数，参照：SETFLAG_ 宏
		 *********************************************************/
		void set(const nstring& value, int32 timeout, int32 flag = SETFLAG_EX);
		/*********************************************************
		  * Fun：置内容
		  * Param：
		  *			value		:		内容
		  ********************************************************/
		newobj::db::redis::RDStat set_nx(const nstring& value);
		/*********************************************************
		 * Fun：取子节点
		 * Param：
		 *			name		：		子节点名称
		 *********************************************************/
		newobj::node* next(const nstring& name);
		/*********************************************************
		 * Fun：取值
		 *********************************************************/
		nstring to_string();
		uint32 to_uint32();
		int32 to_int32();
		uint64 to_uint64();
		int64 to_int64();
		void to_json(json& value);
		/*********************************************************
		 * Fun：删除
		 *********************************************************/
		void del();
		/*********************************************************
		 * Fun：设置过期时间
		 * Desc：设置过期时间后默认删除持久储存
		 *********************************************************/
		void expire(int32 timeout_sec);
		/*********************************************************
		 * Fun：[hash] 取值
		 * Param：
		 *			name		:		hash键
		 *********************************************************/
		const nstring operator[](const nstring& name);
		/*********************************************************
		 * Fun：[hash] 置值
		 * Param：
		 *			name				:		hash键
		 *			value				：		hash值
		 *********************************************************/
		void hset(const nstring& name, const nstring& value);
		/*********************************************************
		 * Fun：[hash] 置值
		 * Param：
		 *			name				:		hash键
		 *			value				：		hash值
		 *********************************************************/
		newobj::db::redis::RDStat hset_nx(const nstring& name, const nstring& value);
		newobj::db::redis::RDStat hset_nx(const uint64& name, const nstring& value);
		newobj::db::redis::RDStat hset_nx(const nstring& name, const json& value);
		newobj::db::redis::RDStat hset_nx(const uint64& name, const json& value);
		/*********************************************************
		 * Fun：[hash] 取值
		 * Param：
		 *			name				:		hash键
		 *			value				：		hash值
		 *********************************************************/
		bool hget(const nstring& name, nstring& value);
		bool hget(const nstring& name, json& value);
		bool hget(const uint64& name, nstring& value);
		bool hget(const uint64& name, json& value);
		/*********************************************************
		 * Fun：[hash] 是否存在
		 *********************************************************/
		 /*void Exist();*/
		friend class NodeCache;
	private:
		void update_sql(const nstring& value);
		bool query_sql(nstring& value);
	private:
		//键名
		nstring m_name;
		//缓存指针
		node* m_next_node_ptr = nullptr;
		//缓存池
		node_cache* m_nodeCache = nullptr;
		//ShaName
		nstring m_sha1_name;
		//表头1字节
		nstring m_pre_1;
	};

}
#endif
#endif
