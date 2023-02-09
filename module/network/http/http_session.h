#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <map>
#include <string>
#include "public/string.h"
#include "public/base.h" 
#include <vector>
#include "util/map.hpp"
#include "util/lock.h"
#include "util/json.h"
#include "util/idx_maker.h"
#include "util/cache.h"
#include "http_interface.h"
namespace newobj
{
	namespace network
	{
		namespace http
		{
            class server;
			struct session_config
			{
				session_config()
				{
					update_sec = 0;
					timeout = 0;
				}
				// 更新时间
				uint64 update_sec;
				// 过期时间
				uint32 timeout;
				// 数据
				newobj::json data;
			};
			class session_mgr;
			class MNETWORK_API session:public newobj::json
			{
			public:
                session();
				~session();
                void init(const nstring& session_name);
				void destory();
				nstring session_id();
                void update();
                friend class session_mgr;
                friend class request;
			private:
                nstring m_session_name;
				session_mgr* m_mgr;
			};
			class session_mgr:public newobj::base, public network::http::interface_
			{
			public:
                session_mgr();
				~session_mgr();
                bool start();
				void close();
				nstring create(int32 timeout_sec);
                bool add(const nstring &session_name, int32 timeout_sec);
                bool exist(const nstring &session_name);
                void set(const nstring &session_name,http::session& session);
                void del(const nstring &session_name);
                bool get(const nstring &session_name,http::session& session);
                void update(const nstring &session_name, int32 timeout_sec);
				void clear();
				newobj::cache* cache();
				friend class session;
			private:
				newobj::idx_maker m_index;
                newobj::cache *m_cache;
			};
		}

	}


}



#endif