#include "log.h"
#include "log/log_slice.h"
#include "util/file.h"
//#if defined(_DEBUG) || defined(RELEASE_LOG_EN)
void newobj::debug::log(const nstring& filepath, const uint32 line, const nstring& content, DebugLogType type)
{
	const nstring path = ".\\debug\\log";

	static log::slice _s_log(path,newobj::log::slice::Type::ST_DAY,"debug_%Y%m%d.log");
	nstring type_string;
	switch (type)
	{
	case newobj::debug::DebugLog_Warn:
		type_string = "WARN";
		break;
	case newobj::debug::DebugLog_Gen:
		type_string = "";
		break;
	case newobj::debug::DebugLog_Success:
		type_string = "Success";
		break;
	case newobj::debug::DebugLog_Error:
		type_string = "Error";
		break;
	default:
		break;
	}
	nstring debug_info;
	if (filepath.length() != 0 || line != 0)
	{
		debug_info = "[" + filepath + "][" + nstring::from(line) + "] ";
	}
	_s_log.write("["+type_string+"] "+ debug_info +time::now_time()+":\t\t"+content+"\r\n");
}
//#endif