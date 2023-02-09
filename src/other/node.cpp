#include "node.h"
#include "log/log_define.h"
#include "util/forstring.h"
#include <string>
#include "util/codec.h"
#include "util/verify.h"
#ifdef LIB_DB_MYSQL
newobj::other::NodeCache::NodeCache(newobj::db::redis::Pool* redisPool, newobj::db::mysql::Pool* sqlPool)
{
	this->m_redis_pool = redisPool;
	this->m_mysql_pool = sqlPool;
	for_l(i, this->m_max)
	{
		
		this->m_queue.Push(Create());
	}
}
newobj::other::NodeCache::~NodeCache()
{
}

newobj::other::Node* newobj::other::NodeCache::Get()
{
	newobj::other::Node* node = nullptr;
	if (this->m_queue.Size() == 0)
	{
		node = Create();
	}
	else
	{
		if (!this->m_queue.Pop(node))
		{
			node = Create();
		}
	}
	return node;
}

void newobj::other::NodeCache::Recover(Node* node)
{
	/*回收node内部的ptr*/
	if(node->m_next_node_ptr!=nullptr)
		Recover(node->m_next_node_ptr);

	if (this->m_queue.Size() > this->m_max)
	{
		delete node;
		return;
	}
	this->m_queue.Push(node);
}

newobj::other::Node* newobj::other::NodeCache::Create()
{
	Node* node = new newobj::other::Node(this);
	if (node == nullptr)
	{
		LOG_EXP(0, "insufficient memory,need:" + nstring::from(sizeof(Node)));
	}
	return node;
}

newobj::other::Node::Node(other::NodeCache *nodeCache)
{
	this->m_nodeCache = nodeCache;
}

newobj::other::Node::~Node()
{
	if (this->m_next_node_ptr != nullptr)
		this->m_nodeCache->Recover(this->m_next_node_ptr);
}

void newobj::other::Node::operator=(const nstring& name)
{
	
	this->m_name = name;
	//this->m_sha1_name = codec::sha1(m_name.c_str(), m_name.length());
	this->m_pre_1.clear();
	/*sha1第一个字节*/
	if (verify::IsEngCode(this->m_sha1_name[0]))
	{
		int one = 0;
		if (this->m_sha1_name[0] >= 65 && this->m_sha1_name[0] <= 90)
		{
			//sha1[0]
			one = (int)this->m_sha1_name[0] + 32;
		}
		else
		{
			one = (int)this->m_sha1_name[0];
		}
		m_pre_1.append({ (char)one });
	}
	else
	{
		m_pre_1.append({ this->m_sha1_name[0] });
	}

}

void newobj::other::Node::operator>>(const nstring& value)
{
	Set(value);
}
void newobj::other::Node::Set(const nstring& value)
{
	//写redis
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	conn.set(this->m_name, value);
	//写mysql
	UpdateSql(value);
}
void newobj::other::Node::Set(const nstring& value, int32 timeout, int32 flag)
{
	//写redis
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	if(timeout!=-1)
		conn.set_timeout(this->m_name, value,timeout, flag);
	else
	{
		
		conn.set(this->m_name, value);
		//写mysql
		UpdateSql(value);
	}
}
newobj::db::redis::RDStat newobj::other::Node::SetNX(const nstring& value)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	return conn.setnx(this->m_name, value);
}

newobj::other::Node* newobj::other::Node::Next(const nstring& name)
{

	this->m_next_node_ptr =  this->m_nodeCache->Get();
	this->m_next_node_ptr->m_name = this->m_name + ":" + name;
	return this->m_next_node_ptr;
}

nstring newobj::other::Node::ToString()
{
	newobj::db::redis::Conn conn_redis((ptr)this->m_nodeCache->m_redis_pool);
	nstring value;
	if (conn_redis.get(m_name, value) == false)
	{
		if (!QuerySql(value))
		{
			//UpdateSql("");
		}
		/*写缓存*/
		conn_redis.set(this->m_name, value);
	}
	return value;
}

uint32 newobj::other::Node::ToUInt32()
{
	return atoi(ToString().c_str());
}

int32 newobj::other::Node::ToInt32()
{
	return atoi(ToString().c_str());
}

uint64 newobj::other::Node::ToUInt64()
{
	return atoll(ToString().c_str());
}

int64 newobj::other::Node::ToInt64()
{
	return atoll(ToString().c_str());
}

void newobj::other::Node::ToJson(json& value)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	conn.get(m_name, value);
}

void newobj::other::Node::Del()
{
	Expire(0);
}

void newobj::other::Node::Expire(int32 timeout_sec)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	conn.expire(m_name, timeout_sec);



	if (this->m_nodeCache->m_mysql_pool == nullptr)
		return;
	db::mysql::Conn conn_sql((ptr)this->m_nodeCache->m_mysql_pool);
	conn_sql.SetSql("DELETE FROM cache_" + m_pre_1 + " WHERE c_name = ? LIMIT 1");
	conn_sql.SetString(1, m_sha1_name);
	conn_sql.UpDate();
}
const nstring newobj::other::Node::operator[](const nstring& name)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	nstring value;
	if (conn.hget(this->m_name, name, value))
		return value;
	return "";
}
void newobj::other::Node::HSet(const nstring& name, const nstring& value)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	conn.hset(this->m_name, name, value);
}
newobj::db::redis::RDStat newobj::other::Node::HSetNX(const nstring& name, const nstring& value)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	return conn.hsetnx(this->m_name, name, value);
	
}
newobj::db::redis::RDStat newobj::other::Node::HSetNX(const uint64& name, const nstring& value)
{
	return HSetNX(nstring::from(name), value);
}
newobj::db::redis::RDStat newobj::other::Node::HSetNX(const nstring& name, const json& value)
{
	return HSetNX(name, value.ToString());
}
newobj::db::redis::RDStat newobj::other::Node::HSetNX(const uint64& name, const json& value)
{
	return HSetNX(nstring::from(name), value.ToString());
}
bool newobj::other::Node::HGet(const nstring& name, nstring& value)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	return conn.hget(this->m_name, name, value);
}
bool newobj::other::Node::HGet(const nstring& name, json& value)
{
	newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
	return conn.hget(this->m_name, name, value);
}
bool newobj::other::Node::HGet(const uint64& name, nstring& value)
{
	return HGet(nstring::from(name), value);
}
bool newobj::other::Node::HGet(const uint64& name, json& value)
{
	return HGet(nstring::from(name), value);
}
//void newobj::other::Node::Exist()
//{
//	//newobj::db::redis::Conn conn((ptr)this->m_nodeCache->m_redis_pool);
//	//conn.(this->m_name, name, value);
//}
void newobj::other::Node::UpdateSql(const nstring& value)
{
	if (this->m_nodeCache->m_mysql_pool == nullptr)
		return;
	db::mysql::Conn conn_sql((ptr)this->m_nodeCache->m_mysql_pool);

	conn_sql.SetAutoCommit();
	conn_sql.SetSql("SELECT id FROM cache_" + m_pre_1 + " WHERE c_name = ? LIMIT 1");
	conn_sql.SetString(1, m_sha1_name);
	conn_sql.Query();
	if (conn_sql.GetResultRow() == 1)
	{
		conn_sql.Next();
		uint32 id = conn_sql.GetUInt(1);
		conn_sql.SetSql("UPDATE cache_" + m_pre_1 + " SET c_value = ? WHERE  id = ? LIMIT 1");
		conn_sql.SetString(1, value);
		conn_sql.SetInt(2, id);
		conn_sql.UpDate();
	}
	else
	{
		conn_sql.SetSql("INSERT INTO cache_" + m_pre_1 + " (c_name,c_value)VALUES(?,?)");
		conn_sql.SetString(1, m_sha1_name);
		conn_sql.SetString(2, value);
		conn_sql.UpDate();
	}
	conn_sql.Commit();
}

bool newobj::other::Node::QuerySql(nstring& value)
{
	if (this->m_nodeCache->m_mysql_pool == nullptr)
		return false;
	//去数据库查询
	db::mysql::Conn conn_sql((ptr)this->m_nodeCache->m_mysql_pool);
	conn_sql.SetSql("SELECT c_value FROM cache_" + m_pre_1 + " WHERE c_name = ? LIMIT 1");
	conn_sql.SetString(1, m_sha1_name);
	conn_sql.Query();
	if (conn_sql.GetResultRow() == 1)
	{
		conn_sql.Next();
		value = conn_sql.GetString(1);
		return true;
	}
	return false;
}
#endif
