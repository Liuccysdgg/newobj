
#include "util/log4.h"
#include <iostream>
#include "util/print.h"
#include "util/time.h"
#include "public/exception.h"
#include <string.h>
#if LIB_LOG4CPLUS
#include "log4cplus/logger.h"             
#include "log4cplus/loggingmacros.h"
#include "log4cplus/configurator.h"
#include "log4cplus/initializer.h"
#include "log4cplus/log4cplus.h"
#include "log4cplus/consoleappender.h"
#include "log4cplus/fileappender.h"
#endif
newobj::log4::log4(const nstring& config_filepath)
{
#if LIB_LOG4CPLUS
    log4cplus::PropertyConfigurator::doConfigure(config_filepath.c_str());
#endif
}

newobj::log4::~log4()
{
}
#if LIB_LOG4CPLUS
inline void append_log(log4cplus::LogLevel level,const char* name,const nstring& value)
{
    log4cplus::Logger::getInstance(name).log(level,value.c_str());
}
#else
inline void append_log(const char* name, const char* value)
{
    printf("%s: %s [%s]\r\n",time::now_time().c_str(), name, value);
}
#endif
newobj::log4& newobj::log4::warn(const nstring& value,const nstring& name)
{
#if LIB_LOG4CPLUS
    append_log(log4cplus::WARN_LOG_LEVEL,name.c_str(),value.c_str());
#else
    append_log(name.c_str(), value.c_str());
#endif
	return *this;
}
newobj::log4& newobj::log4::info(const nstring& value,const nstring& name)
{
#if LIB_LOG4CPLUS
    append_log(log4cplus::INFO_LOG_LEVEL,name.c_str(),value.c_str());
#else
    //append_log(name.c_str(), value.c_str());
#endif
	return *this;
}
newobj::log4& newobj::log4::error(const nstring& value,const nstring& name)
{
#if LIB_LOG4CPLUS
    append_log(log4cplus::ERROR_LOG_LEVEL,name.c_str(),value.c_str());
#else
    append_log(name.c_str(), value.c_str());
#endif
	return *this;
}
newobj::log4& newobj::log4::fatal(const nstring& value,const nstring& name)
{
#if LIB_LOG4CPLUS
    append_log(log4cplus::FATAL_LOG_LEVEL,name.c_str(),value.c_str());
#else
    append_log(name.c_str(), value.c_str());
#endif
	return *this;
}
newobj::log4& newobj::log4::debug(const nstring& value,const nstring& name)
{
#if LIB_LOG4CPLUS
    append_log(log4cplus::DEBUG_LOG_LEVEL,name.c_str(),value.c_str());
#else
    append_log(name.c_str(), value.c_str());
#endif
	return *this;
}
#if 0
void newobj::log4::exp(const nstring& name,const nstring& value,int line,const char* funcname)
{

    log4cplus::Logger::getInstance(name.c_str()).log(log4cplus::FATAL_LOG_LEVEL,value.c_str(),LOG4CPLUS_CALLER_FILE(),line,funcname);
    append_log(log4cplus::FATAL_LOG_LEVEL,name.c_str(),value.c_str());
    trw_str(value);
}
#endif