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
		 * Class：大文件传输服务端
		 ***********************************************************/
		class bigfile_server
		{
		public:
			bigfile_server();
			~bigfile_server();
			/******************************************************************************
			 * Function：启动
			 * Param
			 *			ipadderess			：			IP地址
			 *			port						：			端口
			 ******************************************************************************/
			bool start(const nstring& ipaddress, uint32 port);
			/******************************************************************************
			 * Function：加入文件
			 * Param
			 *			filepath				：			文件路径
			 *			filecode				：			文件代码
			 ******************************************************************************/
			bool append(const nstring& filepath,uint32& filecode);
			friend class bigfile_server_handle;
		private:
			struct FileConf
			{
				newobj::file *file = nullptr;
				nstring path;
			};
			// 文件代码生成器
			newobj::idx_maker m_maker = 1;
			// 文件映射表
			newobj::map<uint32,FileConf> m_map;
			// 文件映射表锁
			newobj::mutex m_map_mutex;
			// 服务端
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