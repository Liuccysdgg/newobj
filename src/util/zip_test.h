#pragma once
#if 0
#define ZIP_STATIC
#include "public/base.h"
#include "libzip/zip.h"
#include <map>
namespace newobj
{
	/**************************************************************************************
	 * Class��ZIP��ѹ��
	 **************************************************************************************/
	class zip : newobj::base
	{
	public:
		/*ZIP�ļ���Ϣ*/
		struct zip_file_info
		{
			zip_file_info()
			{
				index = 0;
				status = false;
			}
			nstring name;
			uint32 index;
			struct zip_stat stat;
			bool status;
		};
	public:
		zip();
		~zip();
		/*��*/
		bool open(const nstring& filepath);
		/*�ر�*/
		void close();
		/*
			ȡ�����ļ���
		*/
		const std::map<uint32, zip_file_info>& files();
		/*д���ļ�*/
		bool write(uint32 index,const nstring& filepath);
		/*�ļ�����*/
		uint32 size();
		/*����ѹ���ļ�*/
		zip_file_info analysis(uint32 index);
		void analysis_noret(uint32 index);
	private:
		// ZIP����ָ��
		struct ::zip* m_zip;
		// �ļ�����
		std::map<uint32, zip_file_info> m_files;
	};
}
#endif