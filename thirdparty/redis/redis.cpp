#ifndef _WIN32
#include "redis.h"
#include "public/define.h"
#include <iostream>
#include "public/exception.h"
#include "public/public.h"


//REDIS_REPLY_STRING  £º 1
//REDIS_REPLY_ARRAY £º 2
//REDIS_REPLY_INTEGER £º3
//REDIS_REPLY_NIL  £º 4
//REDIS_REPLY_STATUS £º 5
//REDIS_REPLY_ERROR £º 6

newobj::redis::conn::conn()
{
    m_handle = nullptr;
}

newobj::redis::conn::~conn()
{
    close();
    //m_pool->recover((void*)this);
}

bool newobj::redis::conn::start(const newobj::redis::conn_info &info)
{
    m_info = info;
    timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    redisContext* context = nullptr;
    if(m_handle == nullptr)
    {
        context = redisConnectWithTimeout(info.ipaddress.c_str(),info.port,timeout);
    }
    if(context == nullptr || context->err != 0)
    {
        if(context != nullptr){
            m_lastErrorDesc = context->errstr;
            redisFree(context);
        }
        return false;
    }
    // ÑéÖ¤ÃÜÂë
    {
        if(info.password != "")
        {
            redisReply* reply = (redisReply*)redisCommand(context, "AUTH %s", info.password.c_str());
            if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
                m_lastErrorDesc = reply->str;
                redisFree(context);
                return false;
            }
            free_reply_object(reply);
        }
    }
    m_handle = context;
    return true;
}

void newobj::redis::conn::close()
{
    if(m_handle != nullptr)
    {
        redisFree((redisContext*)m_handle);
        m_handle = nullptr;
    }
}

void redis::conn::recover()
{

}

void redis::conn::task_out()
{

}
void redis::conn::setnx(const nstring &key, const nstring &value)
{
    nstring errormsg = udpate("SETNX "+key+" "+value);
    f_trw_str(errormsg == "",errormsg);
}

void redis::conn::setxx(const nstring &key, const nstring &value)
{
    nstring errormsg = udpate("SETXX "+key+" "+value);
    f_trw_str(errormsg == "",errormsg);
}

void redis::conn::setex(const nstring &key, const nstring &value, int64 timeout_sec)
{
    nstring errormsg = udpate("SETEX "+key+" "+nstring::from(timeout_sec)+" "+value);
    f_trw_str(errormsg == "",errormsg);
}

void newobj::redis::conn::set(const nstring &key, const nstring &value)
{
    nstring errormsg = udpate("SET "+key+" "+value);
    f_trw_str(errormsg == "",errormsg);
}

void redis::conn::expire(const nstring &key, int64 timeout_sec)
{
    nstring errormsg = udpate("expire "+key+" "+nstring::from(timeout_sec));
    f_trw_str(errormsg == "",errormsg);
}

bool newobj::redis::conn::get(const nstring &key, int64 &value)
{
    bool exist;
    nstring errormsg = query_int("GET "+key,exist,value);
    if(errormsg != "")
    {
        trw_str(errormsg);
    }
    return exist;
}

bool newobj::redis::conn::get(const nstring &key, nstring &value)
{
    bool exist;
    nstring errormsg = query_string("GET "+key,exist,value);
    if(errormsg != "")
    {
        trw_str(errormsg);
    }
    return exist;
}

bool newobj::redis::conn::get(const nstring &key, newobj::json &json)
{
    bool exist;
    nstring value;
    nstring errormsg = query_string("GET "+key,exist,value);
    if(errormsg != "")
    {
        trw_str(errormsg);
    }
    json.parse(value);
    return exist;
}

void redis::conn::del(const nstring &key)
{
    nstring errormsg = udpate("del "+key);
    f_trw_str(errormsg == "",errormsg);
}

bool redis::conn::exists(const nstring &key)
{
    redisReply* reply = (redisReply*)redisCommand((redisContext*)m_handle,"EXISTS %s",key.c_str());
    nstring errormsg = check_reply(reply);

    if(errormsg != "")
    {
        free_reply_object(reply);
        trw_str(errormsg);
    }
    bool exist = reply->type == REDIS_REPLY_INTEGER && reply->integer == 1;
    free_reply_object(reply);
    return exist;
}

bool redis::conn::ttl(const nstring &key, timestamp &sec)
{
    bool exist = false;
    nstring errormsg = query_int("TTL "+key,exist,sec);
    if(errormsg != "")
    {
        trw_str(errormsg);
    }
    return exist;
}

bool redis::conn::pttl(const nstring &key, timestamp &msec)
{
    bool exist = false;
    nstring errormsg = query_int("PTTL "+key,exist,msec);
    if(errormsg != "")
    {
        trw_str(errormsg);
    }
    return exist;
}

bool redis::conn::hdel(const nstring &key,const nstring& field)
{
    redisReply* reply = nullptr;
    nstring errormsg = query("hdel "+key+" "+field,reply);
    if(errormsg != "")
    {
        free_reply_object(reply);
        trw_str(errormsg);
    }
    bool exist = reply->integer == 1;
    free_reply_object(reply);
    return exist;
}

bool redis::conn::hexists(const nstring &key,const nstring& field)
{
    redisReply* reply = nullptr;
    nstring errormsg = query("hexists "+key+" "+field,reply);
    if(errormsg != "")
    {
        free_reply_object(reply);
        trw_str(errormsg);
    }
    bool exist = reply->integer != 0;
    free_reply_object(reply);
    return exist;
}

bool redis::conn::hget(const nstring &key, const nstring &field, nstring &value)
{
    bool exist;
    nstring errormsg = query_string("hget "+key+" "+field,exist,value);
    if(errormsg != "")
    {
        trw_str(errormsg);
    }
    return exist;
}

void redis::conn::hset(const nstring &key, const nstring &field, const nstring &value)
{
    nstring errormsg = udpate("HSET "+key+" "+field+" "+value);
    f_trw_str(errormsg == "",errormsg);
}

nstring newobj::redis::conn::check_reply(void *reply)
{
    if(reply == nullptr){
        std::cout<<((redisContext*)m_handle)->errstr<<std::endl;
        trw_str("reply is nullptr");
    }
    redisReply* rr = (redisReply*)reply;
    if(rr->type == REDIS_REPLY_ERROR)
        return rr->str;
    if(rr->type == REDIS_REPLY_STATUS && nstring(rr->str) != "OK")
        return rr->str;
    return "";
}
#define OUTPUT_REDIS_COMMAND 0

nstring redis::conn::udpate(const nstring &cmd)
{
#if OUTPUT_REDIS_COMMAND == 1
    std::cout<<"[redis] update:"<<cmd.c_str()<<std::endl;
#endif

    redisReply* reply = (redisReply*)redisCommand((redisContext*)m_handle,cmd.c_str());
    nstring errormsg = check_reply(reply);
    free_reply_object(reply);
    if(errormsg != "")
    {
        return errormsg;
    }
    return "";
}

nstring redis::conn::query(const nstring &cmd, redisReply *&reply)
{
#if OUTPUT_REDIS_COMMAND == 1
    std::cout<<"[redis] query:"<<cmd.c_str()<<std::endl;
#endif
    reply = (redisReply*)redisCommand((redisContext*)m_handle,cmd.c_str());
    nstring errormsg = check_reply(reply);
    if(errormsg != "")
    {
        free_reply_object(reply);
        reply = nullptr;
        return errormsg;
    }
    return "";
}

nstring redis::conn::query_int(const nstring &cmd, bool &exist, int64 &value)
{
    redisReply* reply = nullptr;
    nstring errormsg = query(cmd,reply);
    f_ret_str(errormsg == "",errormsg);
    if(reply->type == REDIS_REPLY_NIL)
        exist = false;
    else
        value = reply->integer;
    free_reply_object(reply);
    return true;
}

nstring redis::conn::query_string(const nstring &cmd, bool &exist, nstring &value)
{
    redisReply* reply = nullptr;
    nstring errormsg = query(cmd,reply);
    f_ret_str(errormsg == "",errormsg);
    if(reply->type == REDIS_REPLY_NIL)
        exist = false;
    else
    {
        exist = true;
        value = reply->str;
    }
    free_reply_object(reply);
    return true;
}

void redis::conn::free_reply_object(void *reply)
{
    if(reply != nullptr)
        freeReplyObject(reply);
}

#endif
