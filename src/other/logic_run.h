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
		 * Class：逻辑流程
		 * Desc：设计初衷是为了解决复杂的业务逻辑和相互牵连关系。
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
			 * Function：取标志
			 * Desc
			 *			自定义标志，一般用于标记程序运行节点或位置
			 ********************************************************************/
			nvar flag(const nstring& name);
			/********************************************************************
			 * Function：置标志
			 ********************************************************************/
			bool flag(const nstring& name,const nvar& var);
			/********************************************************************
			 * Function：处理等待
			 * Desc
			 *			通过lambda快捷判断进行等待循环处理
			 * Param
			 *			name						:				标记名称
			 *			wait_fun					：				等待判断函数【返回true，不再执行】
			 *			wait_msec				：				循环延迟时间[默认=0]
			 *			timeout_msec			：				超时时间[默认=0],如设置超时时间，并且超过该时间，则返回false。	
			 ********************************************************************/
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && LIB_QT == 1
			bool wait_thread(const nstring& nam,std::function<bool(nvar)> wait_fun, std::function<void(nvar)> timeout_fun, uint32 wait_msec = 0, uint32 timeout_msec = 0);
			void timeout(const nstring& name, std::function<void(nvar)> wait_fun,uint32 timeout_msec);
			void __add_qt_cb(LogicConf* conf);
#endif
			/********************************************************************
			 * Function：等待标记
			 * Desc
			 *			通过lambda快捷判断进行标记等待
			 * Param
			 *			name						:				标记名称
			 *			wait_fun					：				等待判断函数【返回true，立即返回】
			 *			wait_msec				：				循环延迟时间[默认=0]
			 *			timeout_msec			：				超时时间[默认=0],如设置超时时间，并且超过该时间，则返回false。
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
