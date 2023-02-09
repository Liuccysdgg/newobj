#pragma once
#if 0
#include "network/tcp_server.h"
#include "util/idx_maker.h"
#include "util/json.h"
#include "util/lock.h"
#include "bigfile_common.h"
#include "util/file.h"

namespace newobj
{
	namespace network
	{
		class bigfile_server_handle;
		/***********************************************************
		 * Class�����ļ���������
		 ***********************************************************/
		class bigfile_server
		{
		public:
			bigfile_server();
			~bigfile_server();
			/******************************************************************************
			 * Function������
			 * Param
			 *			ipadderess			��			IP��ַ
			 *			port						��			�˿�
			 ******************************************************************************/
			bool start(const nstring& ipaddress, uint32 port);
			/******************************************************************************
			 * Function�������ļ�
			 * Param
			 *			filepath				��			�ļ�·��
			 *			filecode				��			�ļ�����
			 ******************************************************************************/
			bool append(const nstring& filepath,uint32& filecode);
			friend class bigfile_server_handle;
		private:
			struct FileConf
			{
				newobj::file *file = nullptr;
				nstring path;
			};
			// �ļ�����������
			newobj::idx_maker m_maker = 1;
			// �ļ�ӳ���
			newobj::map<uint32,FileConf> m_map;
			// �ļ�ӳ�����
			newobj::mutex m_map_mutex;
			// �����
			network::tcp::server m_server = network::TTPull;
		};

		class bigfile_server_handle
		{
		public:
			bigfile_server_handle(bigfile_server *server,ReqType type, const newobj::buffer& data,uint64 connID);
			~bigfile_server_handle();
		private:
			void fileinfo();
			void dpakcs();
		private:

			bool reply(const newobj::json & data);
			bool reply(const newobj::buffer& data);
			bool reply(network::ReqType type,const newobj::buffer& data);
		private:
			newobj::buffer m_data;
			ReqType m_type;
			bigfile_server* m_server = nullptr;
			uint64 m_connID;
		};
	}
}



#endif