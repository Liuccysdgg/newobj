#pragma once
#if 0
#include "network/tcp_server.h"
#include "util/idx_maker.h"
#include "util/json.h"
#include "util/lock.h"
// �ӽ�����Կ
#define BIGFILE_DES_KEY "eM5tAL*WeZKML$GH"
// �ļ����ݰ���С
#define BIGFILE_PACKSIZE 40960
// �����������
#define BIGFILE_RQUEST_PS 300
namespace newobj
{
	namespace network
	{
		/*��������*/
		enum ReqType
		{
			// ������
			RT_NONE,
			// ��ȡ�ļ���Ϣ
			RT_GET_FILEINFO,
			// ȡ����
			RT_DPACKS,
			// �������쳣,��ֹ����
			RT_ABORT,
			// �������
			RT_DPACKS_OK
		};
	}
}



#endif