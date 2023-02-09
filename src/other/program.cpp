#include "or_program.h"
#include "util/system.h"
#include "util/time.h"
#if 0
void ___or_program_thread_a3__(other::A3Conf* param,other::program* program)
{
	program->m_mutex.lock();
	param->callMultipled++;
	if (param->fun(program) == false)
		param->deleted = true;
	param->callMultipled--;
	program->m_mutex.unlock();
}
newobj::other::program::program()
{
	::thread::start();
}

newobj::other::program::~program()
{
	::thread::stop();
	::thread::wait();
}

void newobj::other::program::a3(const nstring& name, const std::function<bool(program* program)>& fun, uint32 loopMsec,bool thread, uint32 callMultiple )
{
	std::shared_ptr<A3Conf> conf(new A3Conf);
	conf->name = name;
	conf->fun = fun;
	conf->thread = thread;
	conf->loopMsec = loopMsec;
	conf->preUpdateMsec = 0;
	conf->callMultiple = callMultiple;
	this->m_a3_map.push_back(conf);
}

bool newobj::other::program::run()
{
	/************************************[阿三循环]************************************/
	auto iter = m_a3_map.begin();
	while (iter != m_a3_map.end())
	{
		if (iter->get()->preUpdateMsec == 0 || time::now_msec() - iter->get()->preUpdateMsec >= iter->get()->loopMsec)
		{
			m_mutex.lock();
			iter->get()->preUpdateMsec = time::now_msec();
			m_mutex.unlock();
			if (iter->get()->thread)
			{
				if (iter->get()->callMultipled >= iter->get()->callMultiple)
					continue;	//超过最大调用次数
				std::thread t(___or_program_thread_a3__, iter->get(), this);
				t.detach();
			}
			else
			{
				m_mutex.lock();
				if (iter->get()->fun(this) == false)
					iter->get()->deleted = true;
				else
					iter++;
				m_mutex.unlock();
			}
		}
	}
	m_mutex.lock();
	/*删除已被删除函数*/
	iter = m_a3_map.begin();
	while (iter != m_a3_map.end())
	{
		if (iter->get()->deleted)
			m_a3_map.erase(iter++);
		else
			iter++;
	}
	m_mutex.unlock();
	system::sleep_msec(100);
	return true;
}

#endif