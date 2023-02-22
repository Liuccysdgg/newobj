#pragma once
#if 0
#define ZIP_STATIC
#include "public/base.h"
#include "libzip/zip.h"
#include <map>
namespace newobj
{
	/**************************************************************************************
	 * Class：ZIP解压缩
	 **************************************************************************************/
	class zip : newobj::base
	{
	public:
		/*ZIP文件信息*/
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
		/*打开*/
		bool open(const nstring& filepath);
		/*关闭*/
		void close();
		/*
			取所有文件名
		*/
		const std::map<uint32, zip_file_info>& files();
		/*写到文件*/
		bool write(uint32 index,const nstring& filepath);
		/*文件数量*/
		uint32 size();
		/*分析压缩文件*/
		zip_file_info analysis(uint32 index);
		void analysis_noret(uint32 index);
	private:
		// ZIP操作指针
		struct ::zip* m_zip;
		// 文件内容
		std::map<uint32, zip_file_info> m_files;
	};
}
#endif