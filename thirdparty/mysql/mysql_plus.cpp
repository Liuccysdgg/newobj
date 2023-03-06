#include "mysql_plus.h"
#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/resultset.h"
#include "cppconn/metadata.h"
#include "cppconn/resultset_metadata.h"
#include "cppconn/exception.h"
#include "cppconn/warning.h"
#define CHECK_SQL_CONNECTION if(m_handle == nullptr){trw_str("current connection is invalid");}
#define CHECK_SQL_PPST if(m_handle == nullptr){trw_str("current preparedstatement is invalid");}
#define CHECK_SQL_RESULT if(m_handle == nullptr){trw_str("current resultset is invalid");}

#define CONNECTION ((sql::Connection*)m_handle)
#define PREPARE_STATEMENT ((sql::PreparedStatement*)m_handle)
#define RESULT_SET ((sql::ResultSet*)m_handle)
mysql_plus::conn::conn()
{
    m_handle = nullptr;
    m_ppst = nullptr;
}

mysql_plus::conn::~conn()
{
    close();
}
bool mysql_plus::conn::start(const mysql_conn_info &info)
{
    sql::Driver* driver = get_driver_instance();

    // Configure Connect info
    sql::ConnectOptionsMap opts;
    opts["hostName"] =  info.ipaddress.c_str();
    opts["userName"] = info.username.c_str();
    opts["password"] = info.password.c_str();
    opts["schema"] = info.database.c_str();
    opts["port"] = (int)info.port;
    opts["OPT_CONNECT_TIMEOUT"] = 3;
    opts["OPT_CHARSET_NAME"] = info.charset.c_str();
    opts["OPT_SSL_MODE"] = sql::SSL_MODE_DISABLED;
    opts["OPT_RECONNECT"] = sql::ConnectPropertyVal(true);

    sql::Connection* conn = nullptr;
    try
    {
        conn = driver->connect(opts);
        m_handle = conn;
        return true;
    }
    catch (const sql::SQLException & e)
    {
        delete conn;
        m_lastErrorDesc = e.what();
    }
    return false;
}

void mysql_plus::conn::close()
{
    if(CONNECTION != nullptr){
        delete CONNECTION;
    }
    m_handle = nullptr;
}

void mysql_plus::conn::recover()
{
    clear();
}

void mysql_plus::conn::task_out()
{
    try
    {
        if(CONNECTION->isClosed()){
            if(CONNECTION->reconnect() == false){
                std::cout<<"reconnect failed1."<<std::endl;
            }
        }
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

void mysql_plus::conn::clear()
{
    if(this->m_ppst != nullptr)
        delete this->m_ppst;

    this->m_ppst = nullptr;
}

newobj::mysql_plus::prepare_statement* mysql_plus::conn::setsql(const nstring &sql)
{
    CHECK_SQL_CONNECTION;
    clear();

    m_ppst = new prepare_statement;
    try
    {
        m_ppst->m_handle = CONNECTION->prepareStatement(sql.c_str());
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
    return m_ppst;
}

void mysql_plus::conn::begin(bool autocommit)
{
    try
    {
        CONNECTION->setAutoCommit(autocommit);
    }
    catch (const sql::SQLException & e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

void mysql_plus::conn::commit()
{
    try
    {
        CONNECTION->commit();
    }
    catch (const sql::SQLException & e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

void mysql_plus::conn::rollback()
{
    try
    {
        CONNECTION->rollback();
    }
    catch (const sql::SQLException & e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

mysql_plus::prepare_statement::prepare_statement()
{
    m_result = nullptr;
    m_handle = nullptr;
}

mysql_plus::prepare_statement::~prepare_statement()
{
    clear();
    if(PREPARE_STATEMENT != nullptr)
    {
        try
        {
            while (PREPARE_STATEMENT->getMoreResults()) {
                PREPARE_STATEMENT->getResultSet()->close();
            }
            PREPARE_STATEMENT->close();
            delete PREPARE_STATEMENT;
        }
        catch (const sql::SQLException & e)
        {
            std::cout<<e.what()<<std::endl;
        }
    }
}
#if 0
#define PRINT_DEBUG_SET std::cout<<__FUNCTION__<<"["<<index<<"]: "<<value<<std::endl
#define PRINT_DEBUG_SET_NSTRING std::cout<<__FUNCTION__<<"["<<index<<"]: "<<value.c_str()<<std::endl
#else
#define PRINT_DEBUG_SET
#define PRINT_DEBUG_SET_NSTRING
#endif
void mysql_plus::prepare_statement::set_bigint(uint32 index, const nstring &value)
{

    CHECK_SQL_PPST;
    PRINT_DEBUG_SET_NSTRING;
    PREPARE_STATEMENT->setBigInt(index,value.c_str());
}

void mysql_plus::prepare_statement::set_boolean(uint32 index, bool value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET;
    PREPARE_STATEMENT->setBoolean(index,value);
}

void mysql_plus::prepare_statement::set_datetime(uint32 index, const nstring &value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET_NSTRING;
    PREPARE_STATEMENT->setDateTime(index,value.c_str());
}

void mysql_plus::prepare_statement::set_double(uint32 index, double value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET;
    PREPARE_STATEMENT->setDouble(index,value);
}

void mysql_plus::prepare_statement::set_int32(uint32 index, int32 value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET;
    PREPARE_STATEMENT->setInt(index,value);
}

void mysql_plus::prepare_statement::set_uint32(uint32 index, uint32 value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET;
    PREPARE_STATEMENT->setUInt(index,value);
}

void mysql_plus::prepare_statement::set_int64(uint32 index, int64 value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET;
    PREPARE_STATEMENT->setInt64(index,value);
}

void mysql_plus::prepare_statement::set_uint64(uint32 index, uint64 value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET;
    PREPARE_STATEMENT->setUInt64(index,value);
}

void mysql_plus::prepare_statement::set_null(uint32 index)
{
    trw_str("Not supported temporarily");
    //PREPARE_STATEMENT->setNull(index,value);
}

void mysql_plus::prepare_statement::set_string(uint32 index, const nstring &value)
{
    CHECK_SQL_PPST;
    PRINT_DEBUG_SET_NSTRING;
    PREPARE_STATEMENT->setString(index,value.c_str());
}

void mysql_plus::prepare_statement::clear()
{
    if(m_result != nullptr)
    {
        delete m_result;
    }
    m_result = nullptr;
}

uint64 mysql_plus::prepare_statement::update()
{
    CHECK_SQL_PPST;
    clear();
    try
    {
        return PREPARE_STATEMENT->executeUpdate();
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

mysql_plus::result *mysql_plus::prepare_statement::query()
{
    CHECK_SQL_PPST;
    clear();
    try
    {
        m_result = new mysql_plus::result;
        m_result->m_handle = PREPARE_STATEMENT->executeQuery();
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
    return m_result;
}

mysql_plus::result::result()
{
    m_handle = nullptr;
}

mysql_plus::result::~result()
{
    if(RESULT_SET != nullptr)
    {
        try
        {
            while (RESULT_SET->next())
                RESULT_SET->close();
        }
        catch(const std::exception& e)
        {

        }

        delete RESULT_SET;
    }

}

uint32 mysql_plus::result::field_count()
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getMetaData()->getColumnCount();
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

size_t mysql_plus::result::row_count()
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->rowsCount();
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }

}

bool mysql_plus::result::next()
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->next();
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

int32 mysql_plus::result::get_int32(uint32 index)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getInt(index);
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

int32 mysql_plus::result::get_int32(const nstring &name)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getInt(name.c_str());
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

uint32 mysql_plus::result::get_uint32(uint32 index)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getUInt(index);
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

uint32 mysql_plus::result::get_uint32(const nstring &name)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getUInt(name.c_str());
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

int64 mysql_plus::result::get_int64(uint32 index)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getInt64(index);
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

int64 mysql_plus::result::get_int64(const nstring &name)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getInt64(name.c_str());
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

uint64 mysql_plus::result::get_uint64(uint32 index)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getUInt64(index);
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

uint64 mysql_plus::result::get_uint64(const nstring &name)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getUInt64(name.c_str());
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

nstring mysql_plus::result::get_string(uint32 index)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getString(index).c_str();
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

nstring mysql_plus::result::get_string(const nstring &name)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getString(name.c_str()).c_str();
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

bool mysql_plus::result::get_boolean(uint32 index)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getBoolean(index);
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

bool mysql_plus::result::get_boolean(const nstring &name)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getBoolean(name.c_str());
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

double mysql_plus::result::get_double(uint32 index)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getDouble(index);
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}

double mysql_plus::result::get_double(const nstring &name)
{
    CHECK_SQL_PPST;
    try
    {
        return RESULT_SET->getDouble(name.c_str());
    }
    catch (const sql::SQLException & e)
    {
        trw_str(e.what());
    }
}
