
#pragma once
#if 0
#include "network/tcp_client.h"
#include "bigfile_common.h"
#include "public/string.h"
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
		 * Class�����ļ�����ͻ���
		 ***********************************************************/
		class bigfile_client:newobj::thread
		{
		private:
			/*�ļ���Ϣ*/
			struct FileInfo
			{
				// �ļ���
				nstring name;
				// �ܴ�С
				uint32 size = 0;
				// ������С
				uint32 packsize = 0;
				// �ļ�����
				uint32 filecode = 0;
			};
		public:
			/*������*/
			enum Error
			{
				// ����ʧ��
				ERROR_CONNEDT_FAILED,
				// �ļ�������
				ERROR_FILEINFO_NOTFOUND,
				// �ļ��쳣
				ERROR_FILEINFO_EXP,
				// ����ʧ��
				ERROR_REQUEST_FAILED,
				// �����ļ�����ʧ��
				ERROR_LOCALFILE_CREATE_FAILED
			};
			bigfile_client();
			~bigfile_client();
			/******************************************************************************
			 * Function�����к�
			 * Desc���������Ӳ���ȡ�ļ���Ϣ
			 * Param
			 *			ipadderess			��			IP��ַ
			 *			port						��			�˿�
			 *			privatekey			��			��Ȩ��Կ
			 *			filecode				��			�ļ�ʶ����
			 ******************************************************************************/
			bool hello(const nstring& ipaddress, uint32 port,const nstring& privatekey,uint32 filecode);
			/******************************************************************************
			 * Function����ʼ����
			 * Param
			 *			filepath				��			���ش洢·��
			 *			downloaded		��			��ɻص�[ʧ�ܻ�ɹ����ص�]
			 ******************************************************************************/
			bool download(const nstring& filepath, CALLBACK_BIGFILE_DOWNLOADED downloaded = nullptr, CALLBACK_BIGFILE_DOWNLOADING downloading = nullptr);
			/*�ļ���Ϣ*/
			FileInfo fileinfo();

			friend class bigfile_client_handle;
		private:
			/******************************************************************************
			 * Function����������
			 * Param
			 *			type						��			��������
			 *			data						��			��������
			 ******************************************************************************/
			bool request(ReqType type,const newobj::buffer& data);
			/*�������*/
			bool dpacks(const std::vector<uint32>& packs);
		private:
			// tcp�ͻ���
			network::tcp::client m_client = network::TTPull;
			// ������
			Error m_error;
			// ���̱�־
			ReqType m_flag = RT_NONE;
			// ���̻ظ�
			std::map<ReqType,newobj::json> m_reply;
			// �ļ���Ϣ
			FileInfo m_info;
			// ͨ����
			std::mutex m_mutex;
			// �ļ�
			newobj::file m_file;
			// �����ļ�·��
			nstring m_filepath;
			// �յ�����
			std::map<uint32, bool> m_recvCache;
			// �ص�=>������ɻ�ʧ��
			CALLBACK_BIGFILE_DOWNLOADED m_callback_downloaded = nullptr;
			// �ص�=>���ؽ�����
			CALLBACK_BIGFILE_DOWNLOADING m_callback_downloading = nullptr;
			// ���ذٷֱ�
			double m_down_per = 0.0;
			// �ϴ����ؼ���ʱ��
			timestamp m_down_pre_msec = 0;
			// �ϴ����ؼ�������
			uint32 m_down_pre_size = 0;
			// �ϴ��յ�����ʱ��
			timestamp m_recv_pre_msec = 0;

			uint32 m_down_recv_pack = 0;
		public:
			

			// ͨ�� thread �̳�
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