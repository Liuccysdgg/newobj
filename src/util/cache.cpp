#include "cache.h"
#include "util/file.h"
#include "util/forstring.h"
#include "util/time.h"
#include "util/system.h"
newobj::cache::cache()
{
}

newobj::cache::~cache()
{
}

bool newobj::cache::start(const nstring& local_path)
{
    stop();

    m_local_path = newobj::file::format_slash(local_path).trim_end('\\');
    newobj::file::create_dir(m_local_path,true);
    return ::ithread::start();
}

void newobj::cache::stop()
{
    ::ithread::stop();
    ::ithread::wait();
    
    /*释放内存缓存*/
    {
        ____lock____(m_mutex, true);
        for_iter(iter, m_cache)
            delete iter->second;
        m_cache.clear();
    }

    m_local_path.clear();
}

bool newobj::cache::read(const nstring& name, nstring& value)
{
    ____lock____(m_mutex, true);
    auto iter = m_cache.find(name);
    t_ret_f(iter == m_cache.end());
    /*是否过期*/
    {
        if (iter->second->timeout_sec != -1)
        {
            if (iter->second->update_sec + iter->second->timeout_sec < time::now_sec())
            {
                m_cache.erase(iter);
                return false;
            }
        }
    }
    value = iter->second->value;
    return true;
}

bool newobj::cache::write(const nstring& name,const nstring& value, int32 timeout_sec)
{
    ____lock____(m_mutex, true);
    cache_info* info = nullptr;
    auto iter = m_cache.find(name);
    if (iter == m_cache.end())
    {
        info = new cache_info;
        m_cache.insert(std::pair<nstring, cache_info*>(name, info));
    }
    else
        info = iter->second;
    info->update_sec = time::now_sec();
    if (timeout_sec != -2)
    {
        info->timeout_sec = timeout_sec;
    }
    
    info->value = value;
    return true;
}

bool newobj::cache::exist(const nstring& name)
{
    return _exist(name, true);
}

int32 newobj::cache::expire(const nstring& name)
{
    ____lock____(m_mutex, true);
    auto iter = m_cache.find(name);
    if (iter == m_cache.end())
        return -2;
    
    if (iter->second->timeout_sec == -1)
        return -1;
    timestamp now_time = time::now_sec();
    if (iter->second->update_sec + iter->second->timeout_sec < now_time)
    {
        m_cache.erase(iter);
        return -2;
    }
    return (int32)(iter->second->update_sec + iter->second->timeout_sec - now_time);

}

bool newobj::cache::remove(const nstring& name)
{
    ____lock____(m_mutex, true);
    cache_info* info = nullptr;
    auto iter = m_cache.find(name);
    if (iter == m_cache.end())
        return false;
    m_cache.erase(iter);
    return true;
}

bool newobj::cache::update(const nstring& name)
{
    ____lock____(m_mutex, true);
    auto iter = m_cache.find(name);
    t_ret_f(iter == m_cache.end());
    /*是否过期*/
    {
        if (iter->second->timeout_sec != -1)
        {
            if (iter->second->update_sec + iter->second->timeout_sec < time::now_sec())
            {
                m_cache.erase(iter);
                return false;
            }
        }
    }
    iter->second->update_sec = time::now_sec();
    return true;
}

void newobj::cache::clear()
{
    ____lock____(m_mutex, true);
    for_iter(iter, m_cache)
        delete iter->second;
    m_cache.clear();
}
std::map<nstring, newobj::cache::cache_info*>* newobj::cache::list()
{
    return &m_cache;
}
newobj::mutex* newobj::cache::mutex()
{
    return &m_mutex;
}
bool newobj::cache::_exist(const nstring& name, bool is_lock)
{
    ____lock____(m_mutex, is_lock);
    auto iter = m_cache.find(name);
    t_ret_f(iter == m_cache.end());
    /*是否过期*/
    {
        if (iter->second->timeout_sec != -1)
        {
            if (iter->second->update_sec + iter->second->timeout_sec < time::now_sec())
            {
                m_cache.erase(iter);
                return false;
            }
        }
    }
    return true;
}
bool newobj::cache::run()
{


    m_mutex.lock();
    std::list<nstring> remove_list;
    for_iter(iter, m_cache)
    {
        if (iter->second->timeout_sec == -1)
            continue;
        timestamp now_time = time::now_sec();
        if (iter->second->update_sec + iter->second->timeout_sec < now_time)
            remove_list.push_back(iter->first);
    }
    for_iter(iter, remove_list)
    {
        auto itm = m_cache.find(*iter);
        if (itm != m_cache.end())
            m_cache.erase(itm);
    }
    m_mutex.unlock();




    for (uint32 i = 0; i < 60*2; i++)
    {
        if (::ithread::m_state == 1)
            break;
        system::sleep_msec(500);
    }
    return true;
}
