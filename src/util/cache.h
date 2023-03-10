#pragma once
#include "public/define.h"
#include "util/thread.h"
#include "public/base.h"
#include "util/lock.h"
#include "util/thread.h"
namespace newobj
{
	/*********************************************************************
	 * class：缓存
	 *********************************************************************/
	class NEWOBJ_API cache:public newobj::base,private newobj::ithread
	{
	public:
		struct cache_info
		{
			cache_info()
			{
				update_sec = 0;
				timeout_sec = 0;
			}
			// 创建时间
			timestamp update_sec;
			// 过期时间
			uint32 timeout_sec;
			// 内容
			nstring value;
		};
		cache();
		~cache();
		/***************************************************************************************************
		 * function：启动
		 * param
		 *			local_path						：						缓存保存路径[默认=空]，空则不保存本地。
	     ****************************************************************************************************/
		bool start(const nstring& local_path);
		/***************************************************************************************************
		 * function：停止
		 ****************************************************************************************************/
		void stop();
		/***************************************************************************************************
		 * function：读
		 * param
		 *			name								：						键
		 *			value								：						值
		 ****************************************************************************************************/
		bool read(const nstring& name,nstring& value);
		/***************************************************************************************************
		 * function：写
		 * param
		 *			name								：						键
		 *			value								：						值
		 *			timeout_sec						：						过期时间  -2=不设置
		 ****************************************************************************************************/
		bool write(const nstring& name, const nstring& value,int32 timeout_sec = -2);
		/***************************************************************************************************
		 * function：是否存在
		 * param
		 *			name								：						键
		 ****************************************************************************************************/
		bool exist(const nstring& name);
		/***************************************************************************************************
		 * function：失效时间
		 * param
		 *			name								：						键
		 * return
		 *			>=0 则为失效时间，-1=永不过期 -2=无此键
		 ****************************************************************************************************/
		int32 expire(const nstring& name);
		/***************************************************************************************************
		 * function：删除
		 * param
		 *			name								：						键
		 ****************************************************************************************************/
		bool remove(const nstring& name);
		/***************************************************************************************************
		 * function：更新过期
		 ****************************************************************************************************/
		bool update(const nstring& name);
		/***************************************************************************************************
		 * function：清空
		 ****************************************************************************************************/
		void clear();
		/***************************************************************************************************
		 * function：取MAP
		 ****************************************************************************************************/
		std::map<nstring,cache_info*> *list();
		newobj::mutex* mutex();
	private:
		bool _exist(const nstring& name,bool lock);
		// 通过 ithread 继承
		virtual bool run() override;
	private:
		// 本地缓存路径
		nstring m_local_path;
		// 读写锁
		newobj::mutex m_mutex;
		// 缓存数据
		std::map<nstring, cache_info*> m_cache;

	};
}