#pragma once
#if 0
#include "network/tcp_server.h"
#include "util/idx_maker.h"
#include "util/json.h"
#include "util/lock.h"
// 加解密密钥
#define BIGFILE_DES_KEY "eM5tAL*WeZKML$GH"
// 文件数据包大小
#define BIGFILE_PACKSIZE 40960
// 单次请求包量
#define BIGFILE_RQUEST_PS 300
namespace newobj
{
	namespace network
	{
		/*请求类型*/
		enum ReqType
		{
			// 无请求
			RT_NONE,
			// 获取文件信息
			RT_GET_FILEINFO,
			// 取包组
			RT_DPACKS,
			// 服务器异常,中止传输
			RT_ABORT,
			// 传输完成
			RT_DPACKS_OK
		};
	}
}



#endif