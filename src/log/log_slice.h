#pragma once

#include "public/define.h"
#include "public/string.h"
#include "util/file.h"
#include "util/time.h"
#include "util/lock.h"
#include "util/thread.h"
#include "util/queue.hpp"
#ifdef LIB_QT
#include "QtCore/qfile.h"
#endif
namespace newobj
{
	namespace log
	{
		
		/*********************************************
		 * Class：分片日志
		 *********************************************/
		class NEWOBJ_API slice
		{
		public:
			/*分片日志类型*/
			enum Type
			{
				ST_YEAR,
				ST_MONTH,
				ST_DAY,
				ST_HOUR,
				// 单文件
				ST_ONLY_FILE
			};
		public:
			/*****************************************************
			 * Param
			 *			dirpath			：		目录路径/文件路径
			 *			type				：		循环类型
			 *			format			：		文件格式
			 *										默认：%Y年%m月%d日 %H时%M分%S秒.log
			 *			execute_now	：		立即写入(并发高时为false,队列写入日志)
			 *****************************************************/
			slice(const nstring& dirpath, slice::Type type,const nstring& format = "%Y-%m-%d %H_%M_%S.log");
			~slice() ;
			bool write(const nstring& log);
		private:
			nstring m_dirpath;
			nstring m_format;
			slice::Type m_type;
			newobj::mutex m_mutex;
			file m_file;
			time::datetime m_pre_date;
		};
	}
}
