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
		 * Class����Ƭ��־
		 *********************************************/
		class NEWOBJ_API slice
		{
		public:
			/*��Ƭ��־����*/
			enum Type
			{
				ST_YEAR,
				ST_MONTH,
				ST_DAY,
				ST_HOUR,
				// ���ļ�
				ST_ONLY_FILE
			};
		public:
			/*****************************************************
			 * Param
			 *			dirpath			��		Ŀ¼·��/�ļ�·��
			 *			type				��		ѭ������
			 *			format			��		�ļ���ʽ
			 *										Ĭ�ϣ�%Y��%m��%d�� %Hʱ%M��%S��.log
			 *			execute_now	��		����д��(������ʱΪfalse,����д����־)
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
			time::Time m_pre_date;
		};
	}
}