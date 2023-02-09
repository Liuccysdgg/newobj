#include "inet.h"

newobj::network::inet::inet()
{
	m_sportMode = false;								// 性能模式
	m_sport_policy = 10000;						// 性能模式拒绝策略
																			// 高并发情况下出现的多包阻塞，计算机性能紧张情况下的处理方式
																			// * 0 = 任然执行
																			// * > 0 = 任务数超过该数量后丢弃，默认建议设置为 10000 
	m_hp_ptr = 0;											// HP服务指针
	m_hp_listener_ptr = 0;								// HP服务监听器指针
	m_name;											// 名称

}

ptr newobj::network::inet::hp()
{
	return this->m_hp_ptr;
}

ptr newobj::network::inet::hp_listener()
{
	return this->m_hp_listener_ptr;
}


uint32 newobj::network::inet::sport_poliy()
{
	return m_sport_policy;
}

bool newobj::network::inet::sport_mode()
{
	return m_sportMode;
}

void newobj::network::inet::sport_poliy(uint32 sportPolicy)
{
	this->m_sport_policy = sportPolicy;
}

void newobj::network::inet::sport_mode(bool sportMode)
{
	this->m_sportMode = sportMode;
}

const nstring& newobj::network::inet::name()
{
	return this->m_name;
}

void newobj::network::inet::name(const nstring& name)
{
	this->m_name = name;	
}
