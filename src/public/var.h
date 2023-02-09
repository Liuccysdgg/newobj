#pragma once
#include "define.h"
#include "string.h"
namespace newobj
{
	union nvarVal
	{
		bool v_bool;
		int32 v_int32;
		int64 v_int64;
		float v_float;
		double v_double;
		uint32 v_uint32;
		uint64 v_uint64;
	};
	class NEWOBJ_API nvar
	{
	public:
		nvar();
		~nvar();
		nvar(int32 p);
		nvar(int64 p);
		nvar(uint32 p);
		nvar(uint64 p);
		nvar(float p);
		nvar(double p);
		nvar(bool p);
		nvar(const nstring& str);
		void operator=(int32 p);
		void operator=(int64 p);
		void operator=(uint32 p);
		void operator=(uint64 p);
		void operator=(float p);
		void operator=(double p);
		void operator=(bool p);
		void operator=(const nstring& p);

		NVarType type(NVarType varType = NVT_NotFound);

		operator nstring&();
		operator int32&();
		operator int64&();
		operator uint32&();
		operator uint64&();
		operator float&();
		operator double&();
		operator bool&();

		nstring to_string();
		int32 to_int32();
		int64 to_int64();
		uint32 to_uint32();
		uint64 to_uint64();
		float to_float();
		double to_double();
		bool to_bool();

		bool empty();
	private:
		NVarType m_vartype;
		nvarVal m_val;
		nstring m_val_str;
	};

}