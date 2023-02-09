#include "var.h"
//#include <math.h>
#include "public/exception.h"

newobj::nvar::nvar()
{
	m_vartype = NVarType::NVT_Null;
	m_val.v_uint64 = 0;
}

newobj::nvar::nvar(int32 p)
{
	this->m_vartype = NVT_Int;
	this->m_val.v_int32 = p;
}
newobj::nvar::nvar(int64 p)
{
	this->m_vartype = NVT_LLong;
	this->m_val.v_int64 = p;
}
newobj::nvar::nvar(bool p)
{
	this->m_vartype = NVT_LLong;
	this->m_val.v_bool = p;
}

newobj::nvar::nvar(const nstring& str)
{
	this->m_vartype = NVarType::NVT_String;
	this->m_val_str = str;
}
newobj::nvar::nvar(float p)
{
	this->m_vartype = NVT_Float;
	this->m_val.v_float = p;
}
newobj::nvar::nvar(double p)
{
	this->m_vartype = NVT_Double;
	this->m_val.v_double = p;
}
newobj::nvar::nvar(uint32 p)
{
	this->m_vartype = NVT_UInt;
	this->m_val.v_uint32 = p;
}
newobj::nvar::nvar(uint64 p)
{
	this->m_vartype = NVT_ULLong;
	this->m_val.v_uint64 = p;
}
void newobj::nvar::operator=(int32 p)
{
	this->m_vartype = NVT_Int;
	this->m_val.v_int32 = p;
}


void newobj::nvar::operator=(int64 p)
{
	this->m_vartype = NVT_LLong;
	this->m_val.v_int64 = p;
}
void newobj::nvar::operator=(uint32 p)
{
	this->m_vartype = NVT_UInt;
	this->m_val.v_uint32 = p;
}


void newobj::nvar::operator=(uint64 p)
{
	this->m_vartype = NVT_ULLong;
	this->m_val.v_uint64 = p;
}


void newobj::nvar::operator=(bool p)
{
	this->m_vartype = NVT_Boolean;
	this->m_val.v_bool = p;
}

void newobj::nvar::operator=(const nstring& p)
{
	this->m_vartype = NVarType::NVT_String;
	this->m_val_str = p;
}
NVarType newobj::nvar::type(NVarType varType)
{
	if (varType != NVT_NotFound)
		m_vartype = varType;
	return m_vartype;
}
void newobj::nvar::operator=(double p)
{
	this->m_vartype = NVT_Double;
	this->m_val.v_double = p;
}
void newobj::nvar::operator=(float p)
{
	this->m_vartype = NVT_Float;
	this->m_val.v_float = p;
}
newobj::nvar::~nvar()
{

}

newobj::nvar::operator nstring&()
{
	this->m_vartype = NVarType::NVT_String;
	return this->m_val_str;
}

newobj::nvar::operator int32&()
{
	this->m_vartype = NVT_UInt;
	// TODO: 在此处插入 return 语句
	return this->m_val.v_int32;
}
newobj::nvar::operator int64&()
{
	this->m_vartype = NVT_LLong;
	// TODO: 在此处插入 return 语句
	return this->m_val.v_int64;
}
newobj::nvar::operator uint32&()
{
	this->m_vartype = NVT_UInt;
	// TODO: 在此处插入 return 语句
	return this->m_val.v_uint32;
}
newobj::nvar::operator uint64&()
{
	this->m_vartype = NVT_LLong;
	// TODO: 在此处插入 return 语句
	return this->m_val.v_uint64;
}

newobj::nvar::operator float&()
{
	this->m_vartype = NVT_Float;
	// TODO: 在此处插入 return 语句

	return this->m_val.v_float;
}
newobj::nvar::operator double&()
{
	this->m_vartype = NVT_Double;
	// TODO: 在此处插入 return 语句
	return this->m_val.v_double;
}


newobj::nvar::operator bool&()
{
	this->m_vartype = NVT_Boolean;
	// TODO: 在此处插入 return 语句
	return this->m_val.v_bool;
}

nstring newobj::nvar::to_string()
{
	return m_val_str;
}

int32 newobj::nvar::to_int32()
{
	return (int32)*this;
}

int64 newobj::nvar::to_int64()
{
	return (int64)*this;
}

uint32 newobj::nvar::to_uint32()
{
	return (uint32)*this;
}

uint64 newobj::nvar::to_uint64()
{
	return (uint64)*this;
}

float newobj::nvar::to_float()
{
	return (float)*this;
}

double newobj::nvar::to_double()
{
	return (double)*this;
}

bool newobj::nvar::to_bool()
{
	return (bool)*this;
}

bool newobj::nvar::empty()
{
	return this->m_vartype == NVT_Null;
}
