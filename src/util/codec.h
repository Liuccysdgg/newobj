#pragma once
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include "public/define.h"
#include "public/buffer.h"
#include "string.h"

namespace newobj
{

		namespace codec
		{



			namespace url
			{
				nstring NEWOBJ_API en(const nstring& str);
				nstring NEWOBJ_API de(const nstring& str);
			}
			namespace base64
			{
				nstring NEWOBJ_API en(const nstring& str);
				nstring NEWOBJ_API en(const newobj::buffer& data);
				newobj::buffer NEWOBJ_API de(const nstring& data);
			}

			nstring NEWOBJ_API to_utf8(const nstring& gbk);
			nstring NEWOBJ_API to_gbk(const nstring& utf8);
#ifdef LIB_BOOST
			buffer NEWOBJ_API sha1(const char* buff, uint32 len);
#endif

			namespace des
			{
				/*ECB NoPadding*/
				buffer NEWOBJ_API en(const buffer& src, const nstring& key_param);
				buffer NEWOBJ_API de(const buffer& src, const nstring& key_param);
			}
			namespace aes
			{
				/*256 ECB*/
				buffer NEWOBJ_API en(const buffer& src, const nstring& key_param,int type = 2/*0=128 1=192 2=256*/);
				buffer NEWOBJ_API de(const buffer& src, const nstring& key_param,int type = 2/*0=128 1=192 2=256*/);
			}

			nstring uuid();

			nstring  NEWOBJ_API md5(const nstring& value);
			nstring  NEWOBJ_API md5(const newobj::buffer& value);
#ifdef LIB_HPSOCKET
			newobj::buffer NEWOBJ_API ungzip(const newobj::buffer& data);
			newobj::buffer NEWOBJ_API gzip(const newobj::buffer& data);

			newobj::buffer NEWOBJ_API hp_compress(const newobj::buffer& data);
			newobj::buffer NEWOBJ_API hp_uncompress(const newobj::buffer& data);
#endif
		}
}
