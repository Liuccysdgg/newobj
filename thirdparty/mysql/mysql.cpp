#include "public/define.h"
#include "mysql.h"
#include "util/system.h"
#include "util/forstring.h" 
#include "util/codec.h"
#include "public/exception.h"
#include "log/log_define.h"
#ifdef _WIN32
#define MYSQL_VERSION_5_7_38
#ifdef MYSQL_VERSION_5_7_38
#include "include/5.7.38/mysql.h"
#include "include/5.7.38/errmsg.h"
#endif
#else
#include "/usr/include/mysql/mysql.h"
#endif

namespace newobj
{
    namespace mysql
    {

        conn::conn()
        {
            m_pool = NULL;
            m_handle = NULL;
            m_transaction = NULL;
        }
        conn::~conn()
        {
            close();
        }

        bool conn::start(const mysql_conn_info &info)
        {
            m_info = info;
            if (m_handle == NULL)
            {
                m_handle = mysql_init(NULL);
                if (m_handle == NULL)
                {
                    m_lastErrorDesc = "mysql_init() failed。0x01";
                    return false;
                }
            }
            //徭強嶷銭
            char value = 1;
            mysql_options((MYSQL*)m_handle, MYSQL_OPT_RECONNECT, &value);
            mysql_options((MYSQL*)m_handle, MYSQL_SET_CHARSET_NAME,"utf8");

            if (mysql_ping((MYSQL*)m_handle) != 0)
            {
                if (m_handle == NULL)
                {
                    m_lastErrorDesc = "mysql_init() failed。0x02";
                    return false;
                }
                if (mysql_real_connect((MYSQL*)m_handle, m_info.ipaddress.c_str(), m_info.username.c_str(), m_info.password.c_str(), m_info.database.c_str(), m_info.port, NULL, 0) == NULL)
                {
                    m_lastErrorDesc = mysql_error((MYSQL*)m_handle);
                    return false;
                }
                
                
            }


            return true;
        }
        void conn::close()
        {
            if (m_handle != NULL)
            {
                mysql_close((MYSQL*)m_handle);
                m_handle = NULL;
            }
        }

        void conn::recover()
        {

        }
        void conn::query(const nstring& sql, result& result)
        {
//#ifdef _DEBUG
//            std::cout << "[query] " << sql.c_str() << std::endl;
//#endif
            auto query_result = mysql_real_query((MYSQL*)m_handle, sql.c_str(), sql.length());
            if (query_result != 0)
            {
                trw_str("mysql_real_query :" + nstring::from(query_result)+"。"+error());
            }
            result.m_res = mysql_store_result((MYSQL*)m_handle);
            if (result.m_res == nullptr)
            {
                trw_str("mysql_store_result is nullptr。" + error());
            }
        }
        uint64 conn::update(const nstring& sql)
        {
//#ifdef _DEBUG
//            std::cout << "[update] " << sql.c_str() << std::endl;
//#endif
            auto query_result = mysql_real_query((MYSQL*)m_handle, sql.c_str(), sql.length());
            if (query_result != 0)
            {
                trw_str("mysql_real_query :" + nstring::from(query_result) + "。" + error());
            }
            return mysql_affected_rows((MYSQL*)m_handle);
        }
        void conn::transaction(newobj::mysql::transaction& tran)
        {
            if (tran.m_begin)
            {
                trw_str("transaction in progress");
            }
            tran.m_begin = false;
            tran.m_conn = this;
        }
        uint64 conn::insert_id()
        {
            return mysql_insert_id((MYSQL*)m_handle);
        }
        nstring conn::error()
        {
            return mysql_error((MYSQL*)m_handle);
        }

        result::result()
        {
            m_res = NULL;
            m_field_count = 0;
            m_row_count = 0;
            m_row = NULL;
        }
        result::~result()
        {
            if (m_res != NULL)
            {
                mysql_free_result((MYSQL_RES*)m_res);
                m_res = NULL;
            }
        }
        uint32 result::field_count()
        {
            return mysql_num_fields((MYSQL_RES*)m_res);
        }
        uint64 result::row_count()
        {
            return mysql_num_rows((MYSQL_RES*)m_res);
        }
        nstring result::get(uint32 field)
        {
            if (m_row == NULL)
                return "";
            return m_row[field];
        }
        bool result::next()
        {
            m_row = mysql_fetch_row((MYSQL_RES*)m_res);
            return m_row != NULL;
        }
        transaction::transaction()
        {
            m_conn = nullptr;
            m_begin = false;
        }
        transaction::~transaction()
        {
            if (m_begin)
                rollback();
        }
        void transaction::begin()
        {
            f_trw_str(mysql_autocommit((MYSQL*)m_conn->m_handle,0) == 0, "transaction begin failed。"+m_conn->error());
            m_begin = true;
        }
        void transaction::commit()
        {
            if (m_begin == false)
            {
                trw_str("transaction not opened");
            }
            f_trw_str(mysql_commit((MYSQL*)m_conn->m_handle) == 0, "transaction commit failed。" + m_conn->error());
            m_begin = false;
        }
        void transaction::rollback()
        {
            if (m_begin == false)
            {
                trw_str("transaction not opened");
            }
            f_trw_str(mysql_rollback((MYSQL*)m_conn->m_handle) == 0, "transaction rollback failed。" + m_conn->error());
            m_begin = false;
        }
}
}
