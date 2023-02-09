#include <string>
#include "timeout.h"
#include "system.h"
#include "time.h"
#include "public/exception.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
namespace newobj
{
	
	timeout::timeout(const nstring& name)
	{
		this->m_name = name;
		start();
	}
	timeout::~timeout()
	{
		if (stop() == false)
			abort();
		wait();
	}
	nstring timeout::add(std::function<TimeOutRet(timestamp create, uint32& timeout_msec, nstring id, ptr extra_data)> timeout_callback, uint32 timeout, ptr extra_data, bool lock, bool thread_callback)
	{
		nstring timeoutID = "";
		add(timeoutID, timeout_callback, timeout, extra_data, lock, thread_callback);
		return timeoutID;
	}
	bool timeout::add(nstring& timeoutID, std::function<TimeOutRet(timestamp create, uint32& timeout_msec, nstring id, ptr extra_data)> timeout_callback, uint32 timeout, ptr extra_data, bool lock, bool thread_callback)
	{

		/*组合val*/
		TimeOutCont* toc = new TimeOutCont;
		toc->create = time::now_msec();
		toc->callback_ = timeout_callback;
		toc->extra_data = extra_data;
		toc->timeout_msec = timeout;
		toc->thread_callback = thread_callback;
		if (lock)
			this->m_mutex.lock();

		/*增加 event*/
		if (timeoutID == "")
		{
			for (;;)
			{
				timeoutID =nstring::from(m_indexMaker.create());
				if (this->m_events.find(timeoutID) != this->m_events.end())
					continue;
				this->m_events.insert(std::pair<nstring, TimeOutCont*>(timeoutID, toc));
				if (lock)
					this->m_mutex.unlock();
				return true;
			}
		}
		else
		{
			if (this->m_events.find(timeoutID) != this->m_events.end())
			{
				if (lock)
					this->m_mutex.unlock();
				return false;
			}

			this->m_events.insert(std::pair<nstring, TimeOutCont*>(timeoutID, toc));
			{
				if (lock)
					this->m_mutex.unlock();
				return true;
			}

		}

	}
	bool timeout::del(const nstring& timeoutID)
	{
		____lock____(this->m_mutex,true);
		auto iter = this->m_events.find(timeoutID);
		if (iter == this->m_events.end())
			return false;
		this->m_events.erase(iter);
		return true;
	}
	bool timeout::exist(const nstring& timeoutID, bool lock)
	{
		if (lock)
			this->m_mutex.lock();
		auto ret = this->m_events.end() != this->m_events.find(timeoutID);
		if (lock)
			this->m_mutex.unlock();
		return ret;
	}
	bool timeout::update(const nstring& timeoutID, bool lock)
	{
		if (lock)
			this->m_mutex.lock();
		auto iter = this->m_events.find(timeoutID);
		if (iter == this->m_events.end())
		{
			if (lock)
				this->m_mutex.unlock();
			return false;
		}
		iter->second->create = time::now_msec();
		if (lock)
			this->m_mutex.unlock();
		return true;
	}
	bool timeout::set_timeout(const nstring& timeoutID, uint32 timeout_sec, bool lock)
	{
		if (lock)
			this->m_mutex.lock();
		auto iter = this->m_events.find(timeoutID);
		if (iter == this->m_events.end())
		{
			if (lock)
				this->m_mutex.unlock();
			return false;
		}
		iter->second->create = time::now_msec();
		iter->second->timeout_msec = timeout_sec;
		if (lock)
			this->m_mutex.unlock();
		return true;
	}
	bool timeout::get(const nstring& timeoutID, timestamp& create, uint32& timeout_msec, ptr& extra_data, bool lock)
	{
		if (lock)
			this->m_mutex.lock();
		auto iter = this->m_events.find(timeoutID);
		if (iter == this->m_events.end())
		{
			if (lock)
				this->m_mutex.unlock();
			return false;
		}
		create = iter->second->create;
		timeout_msec = iter->second->timeout_msec;
		extra_data = iter->second->extra_data;
		if (lock)
			this->m_mutex.unlock();
		return true;
	}
	void timeout::clear()
	{
		____lock____(this->m_mutex,true);
		this->m_events.clear();

	}
	bool timeout::out(const nstring& timeoutID, bool curr_thread, bool lock)
	{
		if (lock)
			this->m_mutex.lock();
		auto iter = this->m_events.find(timeoutID);
		if (iter == this->m_events.end())
		{
			if (lock)
				this->m_mutex.unlock();
			return false;
		}
		if (curr_thread)
		{
			iter->second->callback_(iter->second->create, iter->second->timeout_msec, timeoutID, iter->second->extra_data);
			delete iter->second;
			this->m_events.erase(iter);
		}
		else
			iter->second->timeout_msec = 1;
		if (lock)
			this->m_mutex.unlock();
		return true;
	}
	bool timeout::set_extra(const nstring& timeoutID, ptr extra, bool lock)
	{
		if (lock)
			this->m_mutex.lock();
		auto iter = this->m_events.find(timeoutID);
		if (iter == this->m_events.end())
		{
			if (lock)
				this->m_mutex.unlock();
			return false;
		}

		iter->second->extra_data = extra;
		if (lock)
			this->m_mutex.unlock();
		return true;
	}
	bool timeout::get_extra(const nstring& timeoutID, ptr& extra, bool lock)
	{
		if (lock)
			this->m_mutex.lock();
		auto iter = this->m_events.find(timeoutID);
		if (iter == this->m_events.end())
		{
			if (lock)
				this->m_mutex.unlock();
			return false;
		}

		extra = iter->second->extra_data;
		if (lock)
			this->m_mutex.unlock();
		return true;
	}
	uint32 timeout::size()
	{
		return this->m_events.size();
	}
	nstring timeout::create_id()
	{
		nstring timeoutID;
		do
		{
			timeoutID =nstring::from(m_indexMaker.create());
		} while (this->m_events.find(timeoutID) != this->m_events.end());
		return timeoutID;
	}
	void newobj_timeout_thread(void* lpParam)
	{
		struct temp_struct
		{
			nstring id;
			TimeOutCont data;
		};
		temp_struct* ts = (temp_struct*)lpParam;

		ts->data.callback_(ts->data.create, ts->data.timeout_msec, ts->id, ts->data.extra_data);
		delete ts;
	}
	bool timeout::run()
	{
		{
			bool result_callback = false;
			____lock____(this->m_mutex,true);

			do
			{
				result_callback = false;
				timestamp now_time = time::now_msec();
				auto iter = m_events.begin();
				while (iter != m_events.end())
				{
					if (iter->second->timeout_msec + (iter->second->update == 0 ? iter->second->create : iter->second->update) < now_time)
					{

						if (iter->second->callback_ != NULL)
						{
							result_callback = true;

							/*是否线程回调*/
							if (iter->second->thread_callback == true)
							{
								struct temp_struct
								{
									nstring id;
									TimeOutCont data;
								};
								temp_struct* ts = new temp_struct;
								ts->id = iter->first;
								ts->data = *(iter->second);

								newobj::thread::create(newobj_timeout_thread,ts);
							}
							else
							{
								if (iter->second->callback_(iter->second->create, iter->second->timeout_msec, iter->first, iter->second->extra_data)
									== TOR_CONTINUE)
								{
									//更新时间，继续进行过期
									iter->second->create = time::now_msec();
									break;
								}
							}

						}
						delete iter->second;
						this->m_events.erase(iter++);
						if (result_callback)
							break;
					}
					else
					{
						iter++;
					}
				}
			} while (result_callback);

		}
		system::sleep_msec(100);
		return true;
	}
	
	itimer::itimer()
	{
		// 延迟时间
		m_msec = 0;
		// 标志：0=无状态 1=已启动 2=关闭信号
		m_idx = 0;
	}

	itimer::~itimer()
	{
	}

	void itimer::start(uint32 msec)
	{
		m_msec = msec;
		if (m_idx == 0)
			::ithread::start();
	}
	void itimer::stop()
	{
		m_idx = 2;
		while (m_idx != 0)
			system::sleep_msec(1);
		return;
	}
	bool itimer::run()
	{
		if (m_msec <= 100)
		{
			system::sleep_msec(m_msec);
			return on_timer();
		}

		for (size_t i = 0; i < m_msec; i += 100)
		{
			system::sleep_msec(100);
			if (m_idx == 2)
			{
				// 退出定时器
				m_idx = 0;
				return false;
			}
		}
		return on_timer();
	}
	void timer::on_callback(nobject* recver, CALLBACK_TIMER pfun_timer)
	{
		m_recver = recver;
		m_pfun = pfun_timer;
	}
	timer::timer()
	{
		m_pfun = nullptr;
		m_recver = nullptr;
	}
	timer::~timer()
	{
	}
	bool timer::on_timer()
	{
		return (m_recver->*m_pfun)();
	}
}
