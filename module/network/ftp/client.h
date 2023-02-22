#pragma once
#include "network/define.h"
#if USE_NET_FTP
#include "public/base.h"
#include "basetype/string.h"
#include "util/thread.h"
#include "util/file.h"
namespace newobj
{
	namespace network
	{
		namespace ftp
		{
			class client;

			// 下载中
			typedef void(*CALLBACK_FTPCLIENT_DOWNLOADING)(newobj::network::ftp::client* client, uint32 all_size, uint32 down_size, ptr param);
			// 下载完成
			typedef void(*CALLBACK_FTPCLIENT_DOWNLOADED)(newobj::network::ftp::client* client, bool result, ptr param);


			// 下载信息
			struct DownInfo
			{
				DownInfo()
				{
					file = nullptr;
					size = 0;
					fileptr = 0;
					param = 0;
					downloading = nullptr;
					downloaded = nullptr;
					ok = false;
				}
				~DownInfo()
				{
					if (file != nullptr)
					{
						file->close();
						delete file;
					}
				}
				newobj::file* file;
				uint32 size;
				void* fileptr;
				nstring local_path;
				nstring remote_path;
				ptr param;
				CALLBACK_FTPCLIENT_DOWNLOADING downloading;
				CALLBACK_FTPCLIENT_DOWNLOADED downloaded;
				bool ok;
			};



			/************************************************************************
			 * Class：FTP客户端
			 ************************************************************************/
			class MNETWORK_API client :public base, private newobj::ithread
			{
			public:
				client();
				~client();
				bool connect(const nstring& ipaddress, const nstring& username, const nstring& password, ushort port);
				void close();
				bool upload(const nstring& local_filepath, const nstring& remote_filepath);
				bool download(const nstring& local_filepath, const nstring& remote_filepath, ptr param, CALLBACK_FTPCLIENT_DOWNLOADING downloading, CALLBACK_FTPCLIENT_DOWNLOADED downloaded);
				void wait();
				bool download(const nstring& local_filepath, const nstring& remote_filepath);






				bool create_dir(const nstring& remote_path);
				bool delete_dir(const nstring& remote_path);
				bool delete_file(const nstring& remote_path);
			private:
				void* m_ptr;
				DownInfo* m_download;
				CALLBACK_FTPCLIENT_DOWNLOADING m_cb_downloading;
				CALLBACK_FTPCLIENT_DOWNLOADED m_cb_downloaded;

				// 通过 thread 继承
				virtual bool run() override;
			};
		};
	}

}
#endif