#pragma once
#include "public/define.h"

namespace newobj
{
	namespace math
	{
		/*����С����*/
		float  NEWOBJ_API to_fixed(float num, uint32 x);
		double  NEWOBJ_API to_fixed(double num, uint32 x);
		/*ȡС����λ��*/
		uint32  NEWOBJ_API decimal_last_size(float num);
		/*��������*/
		double round(double number, int bits = 0);
		/*ȡ��*/
		int32 to_int(double value);
	}
}
