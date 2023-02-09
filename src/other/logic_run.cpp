#include "logic_run.h"
#include "util/system.h"
#include "util/time.h"
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && defined(LIB_QT)


//超时回调函数指针
newobj::TimeOutRet __logic_run__callback__(
    timestamp create/*创建时间*/,
    uint32& timeout_msec/*过期时间[毫秒]*/,
    nstring id/*超时事件ID*/,
    ptr extra_data/*附加数据*/)
{
    auto conf = (logic_run::LogicConf*)extra_data;
	conf->logic_run->__add_qt_cb(conf);
	return newobj::TOR_CLOSE;
}

#endif

newobj::logic_run::logic_run()
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && defined(LIB_QT)
	:QObject(nullptr)
#endif
{
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && defined(LIB_QT)
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_timer_exec()));
	m_timer.start(500);
#endif
}

newobj::logic_run::~logic_run()
{
}

nvar newobj::logic_run::flag(const nstring& name)
{
	nvar ret;
	m_flags.get(name,ret);
	return ret;
}

bool newobj::logic_run::flag(const nstring& name,const nvar& var)
{
	return m_flags.set(name,var,true);
}
bool newobj::logic_run::wait(const nstring& name, std::function<bool(nvar)> wait_fun, uint32 wait_msec, uint32 timeout_msec)
{
	timestamp in_time = time::now_msec();
	while (!wait_fun(flag(name)))
	{
		if (wait_msec != 0)
			system::sleep_msec(wait_msec);
		if (timeout_msec != 0 && time::now_msec() > timeout_msec + in_time)
			return false;
	}
	return true;
}
#if defined(NEWOBJ_OTHER_LOGICRUN_QT) && defined(LIB_QT)
void newobj::logic_run::on_timer_exec()
{
	LogicConf* conf = nullptr;
	while (this->m_fun.pop(conf))
	{
		if (conf->wait_fun == nullptr)
		{
			conf->timeout_fun(flag(conf->name));
			delete conf;
		}
		else
		{
			if (conf->timeout_msec != 0 && conf->timeout_msec < time::now_msec() - conf->create_time)
			{
				if (conf->timeout_fun != nullptr)
				{
					conf->timeout_fun(flag(conf->name));
				}

				delete conf;
			}
			else
			{
				if (conf->wait_fun(flag(conf->name)))
				{
					delete conf;
				}
				else
				{
					this->m_timeout.add(__logic_run__callback__, conf->wait_msec, (ptr)conf);
				}
			}
		}
		

		
	}
}

void newobj::logic_run::__add_qt_cb(LogicConf* conf)
{
	this->m_fun.push(conf);
}

bool newobj::logic_run::wait_thread(const nstring& name,std::function<bool(nvar)> wait_fun, std::function<void(nvar)> timeout_fun, uint32 wait_msec, uint32 timeout_msec)
{
	auto conf = new LogicConf;
	conf->name = name;
	conf->wait_fun = wait_fun;
	conf->wait_msec = wait_msec;
	conf->timeout_msec = timeout_msec;
	conf->timeout_fun = timeout_fun;
	conf->create_time = time::now_msec();
	conf->logic_run = this;
	this->m_timeout.add(__logic_run__callback__, wait_msec, (ptr)conf);
	return true;
}

void newobj::logic_run::timeout(const nstring& name, std::function<void(nvar)> wait_fun, uint32 timeout_msec)
{
	auto conf = new LogicConf;
	conf->name = name;
	conf->wait_fun = nullptr;
	conf->timeout_msec = timeout_msec;
	conf->timeout_fun = wait_fun;
	conf->create_time = time::now_msec();
	conf->logic_run = this;
	this->m_timeout.add(__logic_run__callback__, timeout_msec, (ptr)conf);
}
#endif