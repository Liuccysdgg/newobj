#pragma once
#include "public/buffer.h"

namespace newobj
{
	namespace brotli
	{
		bool en(const newobj::buffer& src,newobj::buffer& dst);
		bool un(const newobj::buffer& src, newobj::buffer& dst);
	}

}
