#include "extra_data.h"
bool newobj::extra_data::extra(const nstring& name, const nvar& data)
{
	newobj::alock lock(m_mutex, true);
	return m_userData.set(name, data, true);
}

nvar newobj::extra_data::extra(const nstring& name)
{
	newobj::alock lock(m_mutex, true);
	nvar var;
	m_userData.get(name, var);
	return var;
}
