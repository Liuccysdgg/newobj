#pragma once
#include "public/string.h"
#include "util/thread.h"
#include "util/map.hpp"
#include <functional>
#include <vector>
#include <memory>
#ifndef MSVC_2010
namespace newobj
{
	namespace other
	{
		class program;
		/*阿三配置*/ 
		struct A3Conf
		{
			A3Conf()
			{
				loopMsec = 0;
				preUpdateMsec = 0;
				thread = false;
				deleted = false;
				callMultiple = 1;
				callMultipled = 0;
			}
			// 名称
			nstring name;
			// 调用函数
			std::function<bool(program* program)> fun;
			// 循环周期
			timestamp loopMsec;
			// 上一次更新时间
			timestamp preUpdateMsec;
			// 是否线程调用
			bool thread;
			// 删除
			bool deleted;
			// 调用次数
			uint32 callMultiple;
			// 已调用次数
			uint32 callMultipled;
		};
		/**************************************************************************************
		 * Class：程序
		 * Desc：进程单例，为整个程序框架提供支持
		 **************************************************************************************/
		class program :private ithread
		{
		public:
			program();
			~program();
			/*******************************[ 阿三函数 ]****************************************/
			/* 印度阿三挂火车，CPP阿三挂进程。[非线程安全，请仅于初始化时调用。]
			 * 
			 *		name			：			名称
			 *		fun				：			返回false则放弃该函数的后续执行权。返回true下次循环继续执行
			 *      loop_msec	：			循环时长[毫秒]
			 *		thread			：			是否单独线程执行
			 *		callMultiple	：			允许调用次数,该参数仅在 thread=true 有效。
			 */
			void a3(const nstring& name,const std::function<bool(program* program)>& fun,uint32 loopMsec,bool thread = false,uint32 callMultiple = 1);
			/*******************************[ 指针管理 ]****************************************/
			template<typename T>
			T pointer(const nstring& name)
			{
				ptr param;
				if(m_obj_map.get(name, param) == false);
					return (T)nullptr;
				return (T)param;
			}
			template<typename T>
			void pointer(const nstring& name,const T& value)
			{
				m_obj_map.set(name,(ptr)value,true);
			}
		private:
			virtual bool run() override;
		private:
			map<nstring, ptr> m_obj_map;
			std::vector<std::shared_ptr<A3Conf>> m_a3_map;
		public:
			newobj::mutex m_mutex;
		};
	}
}
#endif