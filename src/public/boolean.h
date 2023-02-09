#pragma once
#include "public/string.h"
class boolv
{
public:
	boolv();
	boolv(bool value);
	~boolv();
	void operator=(bool value);
	bool operator==(const boolv& value);
	bool operator==(const bool& value);
	nstring to_string();
private:
	bool m_value;
};

