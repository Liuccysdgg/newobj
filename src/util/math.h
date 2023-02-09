#pragma once
#include "public/define.h"

namespace newobj
{
	namespace math
	{
		/*保留小数点*/
		float  NEWOBJ_API to_fixed(float num, uint32 x);
		double  NEWOBJ_API to_fixed(double num, uint32 x);
		/*取小数点位数*/
		uint32  NEWOBJ_API decimal_last_size(float num);
		/*四舍五入*/
		double round(double number, int bits = 0);
		/*取整*/
		int32 to_int(double value);
	}
}
