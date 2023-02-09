#pragma once
#include "public/string.h"
#include "public/buffer.h"
#include "util/codec.h"
#include "util/time.h"
#ifdef _DEBUG
#define EN_STRING(STRING) nstring(STRING)
#else
#ifdef CODE_EN
#define EN_STRING(STRING) _sdk_safe_define_de_string_("/*PACE_BUFFER_BEGIN---------------------------------------------------------------------------------"##STRING##"-----------------------------------------------------------------------------------PACK_BUFFER_END*/")
#else
#define EN_STRING(STRING) nstring(STRING)
#endif
#endif
#define _E(STRING) EN_STRING(STRING)


inline nstring _sdk_safe_define_de_string_(nstring data)
{
	data = data.trim_end(' ');
	for (uint32 i = 0; i < 365; i++)
	{
		for (uint32 x = 0; x < 50; x++)
		{
			newobj::buffer key_buf;
			key_buf.append(time::format(time::now_sec() - 60 * 60 * 24 * i, "%Y-%m-%d") + nstring::from(x));
			key_buf.append(time::format(time::now_sec() - 60 * 60 * 24 * i, "%Y-%m-%d"));
			nstring key = codec::base64::en(key_buf);
			nstring destring = codec::des::de(codec::base64::de(data), key).to_string();
			if (destring.left(4) == "#00#")
				return destring.right(destring.length() - 4);
		}
	}
	return "";
}