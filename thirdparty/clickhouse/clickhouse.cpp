#if 1
#include "clickhouse.h"
#include "public/define.h"
#include <iostream>
#include "public/exception.h"
#include "public/public.h"



newobj::clickhouse::conn::conn()
{
    m_client = nullptr;
}

newobj::clickhouse::conn::~conn()
{
    close();
}

bool newobj::clickhouse::conn::start(const newobj::clickhouse::conn_info &info)
{    m_option = ::clickhouse::ClientOptions()
        .SetHost(info.ipaddress.c_str())
        .SetPort(info.port)
        .SetUser(info.username.c_str())
        .SetPassword(info.password.c_str())
        .SetDefaultDatabase(info.database.c_str());
	try
	{
	 
    		m_client = new ::clickhouse::Client(::clickhouse::ClientOptions(m_option).SetPingBeforeQuery(false));
	}catch(const std::exception& e){
		m_lastErrorDesc = e.what();
		return false;
	}
    return true;
}

void newobj::clickhouse::conn::close()
{	
	if(m_client != nullptr){
    		delete m_client;
    		m_client = nullptr;
	}
}
::clickhouse::Client* newobj::clickhouse::conn::get(){
	if(m_client == nullptr){
		trw_str("clickhouse client is nullptr");
	}
	return m_client;
}
void newobj::clickhouse::conn::recover()
{

}

void newobj::clickhouse::conn::task_out()
{
	
}
#endif
