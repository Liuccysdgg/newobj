#pragma once
#include "util/lock.h"
#include "util/json.h"
#include "util/file.h"
#include "public/string.h"
#include <map>

namespace newobj
{
    class NEWOBJ_API environment
    {
    public:
        template<typename T>
        T* to(const nstring& name)
        {
            ____lock____(m_mutex, true);
            return (T*)m_map[name];
        }
        template<typename T>
        void set(const nstring name,T* value)
        {
            ____lock____(m_mutex, true);
            m_map[name] = (ptr)value;
        }
        bool save(const nstring& path)
        {
            ____lock____(m_mutex, true);
            newobj::json data;
            data = newobj::json::array;
            for_iter(iter, m_map)
            {
                newobj::json line;
                line["name"] = iter->first;
                line["value"] = nstring::from(iter->second);
                data.push_back(data);
            }
            return newobj::file::write(path, data.to_string());
        }
        void load(const nstring& path)
        {
            ____lock____(m_mutex, true);
            newobj::json data;
            data.parse_file(path);
            for (uint32 i = 0; i < data.size(); i++)
                m_map[data[i]["name"].to<nstring>()] = (uint32)data[i]["value"].to<nstring>().to_uint64();
        }
        void del(const nstring& name)
        {
            ____lock____(m_mutex, true);
            auto iter = m_map.find(name);
            if (iter != m_map.end())
                m_map.erase(iter);
        }
    private:
        newobj::mutex m_mutex;
        std::map<nstring, ptr> m_map;
        nstring m_path;
    };
}
namespace newobj
{
    NEWOBJ_API extern newobj::environment *env;
}
