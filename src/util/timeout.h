#pragma once
#include <string>
#include "public/define.h"
#include "thread.h"
#include "idx_maker.h"
#include<map>
#include <list>
#include "public/define.h"
#include <array>
#include "public/string.h"
#include "lock.h"
namespace newobj
{
	enum TimeOutRet
	{
		TOR_CONTINUE,       //继续进行过期，更新起始时间
		TOR_CLOSE               //结束
	};
	//过期简要
	struct TimeOutBrief
	{
		TimeOutBrief() {
			timeout_go = 0;
			id = 0;
		}
		timestamp timeout_go;   //现行时间大于该时间戳则退出
		uint32 id;
	};
	//事件内容
	struct TimeOutCont
	{
		TimeOutCont()
		{
			create = 0;
			update = 0;
			callback_ = NULL;
			extra_data = 0;
			thread_callback = false;
		}
		timestamp create;
		timestamp update;
		std::function<TimeOutRet(timestamp create/*创建时间*/, uint32& timeout_msec/*过期时间[毫秒]*/, nstring id/*超时事件ID*/, ptr extra_data/*附加数据*/)>  callback_;
		ptr extra_data;
		uint32 timeout_msec;
		bool thread_callback;
	};
	/****************************************************
	* Class：过期管理器
	****************************************************/
	class  NEWOBJ_API timeout :private ithread
	{
	public:
		timeout(const nstring& name = "");
		~timeout();
		/*********************************************************************************
		 * Fun：增加过期事件
		 * Param：
		 *          timeout_callback                 :           过期回调函数
		 *          timeout                               ：         过期事件[毫秒]
		 *          extra_data                           ：         附加数据，一般用于区分。
		 *          lock                                     ：         是否加锁
		 *          thread_callback                   ：         线程内回调
		 * Return:
		 *          nstring ：返回事件ID
		 *********************************************************************************/
		nstring add(std::function<TimeOutRet(timestamp create, uint32& timeout_msec, nstring id, ptr extra_data)> timeout_callback, uint32 timeout, ptr extra_data = 0, bool lock = true, bool thread_callback = false);
		/*********************************************************************************
		 * Fun：增加过期事件
		 * Param：
		 *          timeoutID                           ：         指定的timeouID
		 *          timeout_callback                 :           过期回调函数
		 *          timeout                               ：         过期事件[毫秒]
		 *          extra_data                           ：         附加数据，一般用于区分。
		 *          lock                                     ：         是否加锁
		 *          thread_callback                   ：         线程内回调
		 *********************************************************************************/
		bool add(nstring& timeoutID, std::function<TimeOutRet(timestamp create, uint32& timeout_msec, nstring id, ptr extra_data)> timeout_callback, uint32 timeout_msec, ptr extra_data = 0, bool lock = true, bool thread_callback = false);
		/*********************************************************************************
		 * Fun：删除过期事件
		 * Param：
		 *          id                                       :           事件ID
		 * Return：
		 *      true = 存在并已删除
		 *      false = 不存在或已过期
		 *********************************************************************************/
		bool del(const nstring& timeoutID);
		/*********************************************************************************
		 * Fun：是否存在过期事件
		 * Param：
		 *          id                                       :           事件ID
		 *********************************************************************************/
		bool exist(const nstring& timeoutID, bool lock = true);
		/*********************************************************************************
		 * Fun：更新过期
		 * Param：
		 *          id                                       :           事件ID
	   *********************************************************************************/
		bool update(const nstring& timeoutID, bool lock = true);
		/*********************************************************************************
		 * Fun：修改过期时间
		 * Param：
		 *          id                                       :           事件ID
		 *			timeout_msec					  ：		 过期时间
	   *********************************************************************************/
		bool set_timeout(const nstring& timeoutID,uint32 timeout_sec, bool lock = true);
		/*********************************************************************************
		 * Fun：获取过期事件
		 * Param：
		 *          id                                                  :           事件ID
		 *          [传址]timeout_callback                 :           过期回调函数
		 *          [传址]timeout_msec                     ：         过期事件[毫秒]
		 *          [传址]extra_data                           ：         附加数据，一般用于区分。
		 *          lock                                              ：         是否加锁
		 *********************************************************************************/
		bool get(const nstring& timeoutID, timestamp& create/*创建时间*/, uint32& timeout_msec/*过期时间[毫秒]*/, ptr& extra_data/*附加数据*/, bool lock = true);
		void clear();
		bool out(const nstring& timeoutID, bool curr_thread = false, bool lock = true);
		bool set_extra(const nstring& timeoutID, ptr extra, bool lock = true);
		bool get_extra(const nstring& timeoutID, ptr& extra, bool lock = true);
		uint32 size();
		/*仅生成ID*/
		nstring create_id();
	private:
		// 通过 Thread 继承
		virtual bool run() override;
	private:
		idx_maker m_indexMaker;            //事件ID生成器
		nstring m_name;                                 //别名
		std::map<nstring, TimeOutCont*> m_events;
	public:
		newobj::mutex m_mutex;
	};
	/****************************************************
	 * Class：定时器接口
	 ****************************************************/
	class NEWOBJ_API itimer :private ithread
	{
	public:
		itimer();
		~itimer();
		/*******************************************************************
		 * Function：启动
		 * Param
		 *          msec                    ：           毫秒
		 *******************************************************************/
		void start(uint32 msec);
		/*******************************************************************
		 * Function：停止
		 *******************************************************************/
		void stop();
	protected:
		virtual bool on_timer() = 0;
	private:
		// 延迟时间
		uint32 m_msec;
		// 标志：0=无状态 1=已启动 2=关闭信号
		int32 m_idx;
	private:
		// 通过 Thread 继承
		virtual bool run() override;

	};
}
