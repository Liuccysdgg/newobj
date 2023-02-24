#if 0
#pragma once
#include "util/map.hpp"
#include "public/var.h"
#include <functional>
#include "util/queue.hpp"
#define NEWOBJ_OTHER_LOGICRUN_QT
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && LIB_QT == 1
#include "QtCore/qtimer.h"
#endif
namespace newobj
{
#ifdef NEWOBJ_OTHER_LOGICRUN_QT
	typedef std::function<void(ptr, nstring)> REPLY_CALLBACK;
	
#endif
		/************************************************************************
		 * Class���߼�����
		 * Desc����Ƴ�����Ϊ�˽�����ӵ�ҵ���߼����໥ǣ����ϵ��
		 ************************************************************************/
		class logic_run
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && LIB_QT == 1
			:QObject
#endif
		{
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && LIB_QT == 1
			Q_OBJECT
#endif
		public:
			struct LogicConf
			{
				LogicConf()
				{
					wait_msec = 0;
					timeout_msec = 0;
					create_time = 0;
					logic_run = nullptr;
					param = nullptr;
				}
				nstring name;
				std::function<bool(nvar)> wait_fun;
				std::function<void(nvar)> timeout_fun;
				timestamp wait_msec;
				timestamp timeout_msec;
				timestamp create_time;
				class logic_run* logic_run;
				void* param;
			};
			logic_run();
			~logic_run();
			/********************************************************************
			 * Function��ȡ��־
			 * Desc
			 *			�Զ����־��һ�����ڱ�ǳ������нڵ��λ��
			 ********************************************************************/
			nvar flag(const nstring& name);
			/********************************************************************
			 * Function���ñ�־
			 ********************************************************************/
			bool flag(const nstring& name,const nvar& var);
			/********************************************************************
			 * Function������ȴ�
			 * Desc
			 *			ͨ��lambda����жϽ��еȴ�ѭ������
			 * Param
			 *			name						:				�������
			 *			wait_fun					��				�ȴ��жϺ���������true������ִ�С�
			 *			wait_msec				��				ѭ���ӳ�ʱ��[Ĭ��=0]
			 *			timeout_msec			��				��ʱʱ��[Ĭ��=0],�����ó�ʱʱ�䣬���ҳ�����ʱ�䣬�򷵻�false��	
			 ********************************************************************/
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && LIB_QT == 1
			bool wait_thread(const nstring& nam,std::function<bool(nvar)> wait_fun, std::function<void(nvar)> timeout_fun, uint32 wait_msec = 0, uint32 timeout_msec = 0);
			void timeout(const nstring& name, std::function<void(nvar)> wait_fun,uint32 timeout_msec);
			void __add_qt_cb(LogicConf* conf);
#endif
			/********************************************************************
			 * Function���ȴ����
			 * Desc
			 *			ͨ��lambda����жϽ��б�ǵȴ�
			 * Param
			 *			name						:				�������
			 *			wait_fun					��				�ȴ��жϺ���������true���������ء�
			 *			wait_msec				��				ѭ���ӳ�ʱ��[Ĭ��=0]
			 *			timeout_msec			��				��ʱʱ��[Ĭ��=0],�����ó�ʱʱ�䣬���ҳ�����ʱ�䣬�򷵻�false��
			 ********************************************************************/
			bool wait(const nstring& name, std::function<bool(nvar)> wait_fun, uint32 wait_msec = 0, uint32 timeout_msec = 0);
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && LIB_QT == 1
		private slots:
			void on_timer_exec();
#endif
		private:

#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && LIB_QT == 1
			QTimer m_timer;
			queue<LogicConf*> m_fun;
			newobj::timeout m_timeout;
#endif
			map<nstring, nvar> m_flags;
		};
}
#endif
