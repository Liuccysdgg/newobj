#include "inet.h"

newobj::network::inet::inet()
{
	m_sportMode = false;								// ����ģʽ
	m_sport_policy = 10000;						// ����ģʽ�ܾ�����
																			// �߲�������³��ֵĶ����������������ܽ�������µĴ���ʽ
																			// * 0 = ��Ȼִ��
																			// * > 0 = ����������������������Ĭ�Ͻ�������Ϊ 10000 
	m_hp_ptr = 0;											// HP����ָ��
	m_hp_listener_ptr = 0;								// HP���������ָ��
	m_name;											// ����

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
