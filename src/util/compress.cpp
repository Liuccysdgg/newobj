#include "compress.h"
#include "hpsocket/hpsocket.h"
bool newobj::brotli::en(const newobj::buffer& src, newobj::buffer& dst)
{
	t_ret_f(src.length() == 0);
	
	DWORD length = SYS_BrotliGuessCompressBound((DWORD)src.length());
	dst.setsize(length);
	auto result = SYS_BrotliCompress((const BYTE*)src.data(), (DWORD)src.length(), (BYTE*)dst.data(), length);
	if (result == 0)
	{
		dst = dst.sub(0, length);
		return true;
	}
	else if (result == -3)
		return false;
	else if (result == -5)
		return false;
	return false;
}

bool newobj::brotli::un(const newobj::buffer& src, newobj::buffer& dst)
{
	t_ret_f(src.length() == 0);
	for (uint32 i = 0; i < 1000; i++)
	{
		dst.setsize((i + 1) * src.length());
		DWORD length = 0;
		auto result = SYS_BrotliUncompress((const BYTE*)src.data(), (DWORD)src.length(), (BYTE*)dst.data(), length);
		if (result == 0)
		{
			dst = dst.sub(0, length);
			return true;
		}
		else if (result == -3)
			return false;
		else if (result == -5)
			continue;
	}
	return false;
}

