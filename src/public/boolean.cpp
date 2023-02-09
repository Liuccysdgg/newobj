#include "boolean.h"
#include "exception.h"
#include "util/forstring.h"

boolv::boolv()
{
	m_value = false;
}
boolv::boolv(bool value)
{
	operator=(value);
}
boolv::~boolv()
{
}

void boolv::operator=(bool value)
{
	this->m_value = value;
}

bool boolv::operator==(const boolv& value)
{
	return m_value == value.m_value;
}

bool boolv::operator==(const bool& value)
{
	return m_value == value;
}

nstring boolv::to_string()
{
	return m_value==true?"true":"false";
}

