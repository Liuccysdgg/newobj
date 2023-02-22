#include "idx_maker.h"
namespace newobj
{
	idx_maker::idx_maker(uint32 index)
	{
		this->m_index = index;
	}
	idx_maker::~idx_maker()
	{
	}
	uint32 idx_maker::create()
	{

		uint32 ret;
		this->m_mutex.lock();
		ret = this->m_index;
		this->m_index++;
		this->m_mutex.unlock();
		return ret;
	}
}
