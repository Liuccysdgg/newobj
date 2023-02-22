#pragma once
#include "basetype/string.h"
#include "public/struct.h"
#include "public/buffer.h"
#include <vector>
namespace newobj
{
	namespace system
	{
		/*������Ϣ*/
		struct NetCard
		{
			//������
			nstring name; 
			//IP��ַ
			nstring ipaddress;
			//��������
			nstring netmask;
			//����
			nstring gateway;
			//Mac
			nstring mac;
		};

		/*˯��*/
		void  NEWOBJ_API sleep_msec(unsigned int time);


		/*ִ��shell/cmd*/
		int32 NEWOBJ_API shell(const nstring& cmd, nstring& result, const nstring& temp_filepath);
		/*ִ��shell/cmd*/
		nstring NEWOBJ_API shell(const nstring& cmd);

		typedef void(*CALLBACK_SHELL)(nstring callback, bool close, void* param);
		/*ִ��shell/cmd*/
		bool NEWOBJ_API shell(const nstring& cmd, CALLBACK_SHELL callback, void* param);
		/*ȡ����Ŀ¼*/
		nstring NEWOBJ_API current_dir();
		/*ȡ����·��*/
		nstring NEWOBJ_API current_filepath();
		// ȡ�û�Ŀ¼
		nstring NEWOBJ_API currentuser_path();
		/*��ȡĿ¼���ļ�*/
		//void getFiles(const nstring& rootPath, std::vector<nstring>& ret, std::vector<nstring>& name)
		/*ȡ��Χ�����*/
		int32 NEWOBJ_API random(int32 min, int32 max, int32 rand_idx = 0);
		/*���ɿ�ݷ�ʽ*/
		bool  NEWOBJ_API link(const nstring& srcpath, const nstring& dstpath);
#ifdef _WIN32
		/*ȡ����·��*/
		nstring NEWOBJ_API desktop_path();
#endif
#ifdef _WIN32
		/*�ü������ı�*/
		void NEWOBJ_API clipboard(const nstring& value);
#endif
		/******************************************[ ��Ļ��� ]************************************************/
		namespace screen
		{
			/*ȡ��ʵ��Ļ�豸��С*/
			Size NEWOBJ_API device_size();
			/*
				ȡ��������С
				Desc��������������
			*/
			Size NEWOBJ_API work_size();
			/*���÷ֱ���*/
			bool NEWOBJ_API set(const Size &size);

		}
		/******************************************[ ������� ]************************************************/
		namespace netcard
		{
			/*��ȡ������ַ*/
			nstring  NEWOBJ_API mac(nstring name = "");
			/*��ȡ�����б�*/
			std::vector<NetCard>  NEWOBJ_API list();
			/*��������*/
			bool  NEWOBJ_API set(const NetCard& netCard);
		}
		/******************************************[ CPU��� ]************************************************/
		namespace cpu
		{
			
		}
		/******************************************[ Ӳ����� ]************************************************/
		namespace hardware
		{
			// �Կ���Ϣ
			std::vector<GraphicsCard> NEWOBJ_API graphicscard();
			// CPU��Ϣ
			Cpu  NEWOBJ_API cpu();
		}
		/******************************************[ ϵͳ���� ]************************************************/
		namespace tool
		{
			/*
				����Զ�������ļ�
				@savepath									��			�����ַ
				@ipaddress									��			IP��ַ
				@username									��			�˺�
				@password									��			����
				@port											��			�˿�
				@mod											��			���ڻ�
				@color_15_16_24_32					��			ɫ��
				@width										��			���
				@height										��			�߶�
				@disk											��			ʹ��Ӳ��
				@compression								��			ѹ������
				@displayconnectionbar				��			ȫ����ʾ������
				@wallpaper									��			��ʾ���汳��
				@themes										��			��������
				@autoreconnection						��			�Զ�����
			*/
			bool NEWOBJ_API mstsc(
				const nstring& savepath,
				const nstring& ipaddress,
				const nstring& username,
				const nstring& password,
				uint32 port,
				bool mod,
				uint32 color_15_16_24_32,
				uint32 width,
				uint32 height,
				bool disk = true,
				bool compression = true,
				bool displayconnectionbar = true,
				bool wallpaper = true,
				bool themes = true,
				bool autoreconnection = true
			);
		}



		namespace os
		{
			/*ȡϵͳ�汾ID*/
			nstring NEWOBJ_API version();
		}
		/*ȡ��ʱĿ¼*/
		nstring NEWOBJ_API temp_path();

		/*���ɱ���Ψһ���к�*/
		void NEWOBJ_API serial_number(nstring& sn1, nstring& sn2, nstring& sn3);

		/*�����б�*/
		std::vector<nstring> NEWOBJ_API disk_list();

		/*��������Դ*/
		bool NEWOBJ_API read_res(uint32 resname,const nstring& type,newobj::buffer& data);
	}
}
