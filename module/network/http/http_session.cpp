#include "http_session.h"
#if USE_NET_HTTP_WEBSITE
#include "util/codec.h"
#include "util/system.h"
#include "util/time.h"
#include "util/file.h"
#include "http_server.h"
#include "public/exception.h"
#include "http_center.h"
namespace newobj
{
	namespace network
	{
        namespace http
        {
            session::session()
            {
                m_mgr = nullptr;
            }
            session::~session()
            {
                m_mgr->set(m_session_name,*this);
            }
            void session::init(const nstring& session_name)
            {
                if (this->m_mgr == nullptr)return;


                ::newobj::json::clear();
                this->m_session_name = session_name;
                if (this->m_session_name == "")
                {
                    this->m_session_name = this->m_mgr->create(60*30);
                    return;
                }
                if (this->m_mgr->get(this->m_session_name, *this) == false)
                {
                    this->m_mgr->add(this->m_session_name,60 * 30);
                }
            }
            void session::destory()
            {
                this->m_mgr->del(m_session_name);
            }

            nstring session::session_id()
            {
                return m_session_name;
            }
            void session::update()
            {
                this->m_mgr->update(this->m_session_name,30*60);
            }
            session_mgr::session_mgr()
            {
                m_cache = nullptr;
            }
            session_mgr::~session_mgr()
            {
                close();
            }
            bool session_mgr::start()
            {
                if (m_cache != nullptr)
                {
                    m_lastErrorDesc = "session管理器不允许重复加载";
                    return false;
                }
                m_cache = new newobj::cache;
                return m_cache->start(".\\cache");
            }
            void session_mgr::close()
            {
                if (m_cache != nullptr)
                {
                    delete (newobj::cache*)m_cache;
                    m_cache = nullptr;
                }
            }
            nstring session_mgr::create(int32 timeout_sec)
            {
                nstring session_name = codec::md5(nstring::from(time::now_sec()) + nstring::from(m_index.create()));
                m_cache->write(session_name, "{}", timeout_sec);
                return session_name;
            }
            bool session_mgr::add(const nstring& session_name, int32 timeout_sec)
            {
                return m_cache->write(session_name, "{}", timeout_sec);
            }
            bool session_mgr::exist(const nstring& session_name)
            {
                return m_cache->exist(session_name);
            }
            void session_mgr::set(const nstring& session_name, http::session& session)
            {
                m_cache->write(session_name, session.to_string());
            }
            void session_mgr::del(const nstring& session_name)
            {
                m_cache->remove(session_name);
			}
            bool session_mgr::get(const nstring &session_name,http::session& session)
			{
                nstring value;
                f_ret_f(m_cache->read(session_name, value));
                session.parse(value);
                return true;
			}
            void session_mgr::update(const nstring &session_name,int32 timeout_sec)
			{
                m_cache->update(session_name);
			}
			void session_mgr::clear()
			{
                m_cache->clear();
			}
            newobj::cache* session_mgr::cache()
            {
                return m_cache;
            }
		}


	}
}
#endif