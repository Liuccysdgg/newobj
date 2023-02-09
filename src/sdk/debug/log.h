#pragma once
#include "public/string.h"
#ifdef _DEBUG
#define DEBUG_LOG(CONTENT,TYPE) newobj::debug::log(__FILE__,__LINE__,CONTENT,TYPE)
#else
#define DEBUG_LOG(CONTENT,TYPE)
#endif
#ifdef RELEASE_LOG_EN
#define RELEASE_LOG(CONTENT,TYPE) newobj::debug::log("","",CONTENT,TYPE)
#else
#define RELEASE_LOG(CONTENT,TYPE)
#endif
/*开发调试日志组件*/
namespace newobj
{
	namespace debug
	{
		enum DebugLogType
		{
			DebugLog_Warn,
			DebugLog_Gen,
			DebugLog_Success,
			DebugLog_Error
		};
		void log(const nstring& filepath,const uint32 line,const nstring& content, DebugLogType type);
	}
}