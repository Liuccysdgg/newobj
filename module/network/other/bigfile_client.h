
#pragma once
#if 0
#include "network/tcp_client.h"
#include "bigfile_common.h"
#include "basetype/string.h"
#include <map>
#include "util/lock.h"
#include "util/file.h"
#include "util/thread.h"
#include <mutex>
namespace newobj
{
	namespace network
	{
		
		class bigfile_client_handle;
		class bigfile_client;

		typedef void(*CALLBACK_BIGFILE_DOWNLOADED)(network::bigfile_client* client);
		typedef void(*CALLBACK_BIGFILE_DOWNLOADING)(network::bigfile_client* client,double download_per,uint32 download_msec_size);
		/***********************************************************
		 * Class：大文件传输客户端
		 ***********************************************************/
		class bigfile_client:newobj::thread
		{
		private:
			/*文件信息*/
			struct FileInfo
			{
				// 文件名
				nstring name;
				// 总大小
				uint32 size = 0;
				// 单包大小
				uint32 packsize = 0;
				// 文件代码
				uint32 filecode = 0;
			};
		public:
			/*错误码*/
			enum Error
			{
				// 连接失败
				ERROR_CONNEDT_FAILED,
				// 文件不存在
				ERROR_FILEINFO_NOTFOUND,
				// 文件异常
				ERROR_FILEINFO_EXP,
				// 请求失败
				ERROR_REQUEST_FAILED,
				// 本地文件创建失败
				ERROR_LOCALFILE_CREATE_FAILED
			};
			bigfile_client();
			~bigfile_client();
			/******************************************************************************
			 * Function：打招呼
			 * Desc：建立连接并获取文件信息
			 * Param
			 *			ipadderess			：			IP地址
			 *			port						：			端口
			 *			privatekey			：			鉴权密钥
			 *			filecode				：			文件识别码
			 ******************************************************************************/
			bool hello(const nstring& ipaddress, uint32 port,const nstring& privatekey,uint32 filecode);
			/******************************************************************************
			 * Function：开始下载
			 * Param
			 *			filepath				：			本地存储路径
			 *			downloaded		：			完成回调[失败或成功均回调]
			 ******************************************************************************/
			bool download(const nstring& filepath, CALLBACK_BIGFILE_DOWNLOADED downloaded = nullptr, CALLBACK_BIGFILE_DOWNLOADING downloading = nullptr);
			/*文件信息*/
			FileInfo fileinfo();

			friend class bigfile_client_handle;
		private:
			/******************************************************************************
			 * Function：请求数据
			 * Param
			 *			type						：			请求类型
			 *			data						：			请求数据
			 ******************************************************************************/
			bool request(ReqType type,const newobj::buffer& data);
			/*请求组包*/
			bool dpacks(const std::vector<uint32>& packs);
		private:
			// tcp客户端
			network::tcp::client m_client = network::TTPull;
			// 错误码
			Error m_error;
			// 流程标志
			ReqType m_flag = RT_NONE;
			// 流程回复
			std::map<ReqType,newobj::json> m_reply;
			// 文件信息
			FileInfo m_info;
			// 通用锁
			std::mutex m_mutex;
			// 文件
			newobj::file m_file;
			// 本地文件路径
			nstring m_filepath;
			// 收到缓存
			std::map<uint32, bool> m_recvCache;
			// 回调=>下载完成或失败
			CALLBACK_BIGFILE_DOWNLOADED m_callback_downloaded = nullptr;
			// 回调=>下载进行中
			CALLBACK_BIGFILE_DOWNLOADING m_callback_downloading = nullptr;
			// 下载百分比
			double m_down_per = 0.0;
			// 上次下载计算时间
			timestamp m_down_pre_msec = 0;
			// 上次下载计算流量
			uint32 m_down_pre_size = 0;
			// 上次收到数据时间
			timestamp m_recv_pre_msec = 0;

			uint32 m_down_recv_pack = 0;
		public:
			

			// 通过 thread 继承
			virtual bool run() override;

		};



		class bigfile_client_handle
		{
		public:
			bigfile_client_handle(bigfile_client* client, ReqType type, const newobj::buffer& data);
			~bigfile_client_handle();
		private:
			void fileinfo();
			void dpacks();
			void	dpacks_ok();
		private:
			bool reply(const newobj::json& data);
			bool reply(const newobj::buffer& data);
		private:
			newobj::buffer m_data;
			ReqType m_type;
			bigfile_client* m_client = nullptr;
			newobj::json m_json;
		};

	}
}

#endif