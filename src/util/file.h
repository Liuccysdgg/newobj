#ifndef _UTIL_FILE_H_
#define _UTIL_FILE_H_
#include <string>
#include <stdio.h>
#include "public/define.h"
#include "public/buffer.h"
#include <vector>
#include <map>
#include <list>
namespace newobj
{
		enum open_type {
			// 只读
			OT_R,
			// 读写+创建
			OT_RW_C
		}; 
		/*****************************************************************************************
		 * Class：文件操作类
		 ****************************************************************************************/
		class NEWOBJ_API file 
		{
		public:
			file();
			~file();
			bool open(const nstring& filepath,open_type type = OT_RW_C);
			bool read(uint64 idx, uint64 len, char* buf) const;
			uint64 read_rsize(uint64 idx, uint64 len, char* buf) const;
			bool write(const char* buf, size_t len);
			bool write(const nstring& value);
			bool write(const buffer& value);
			bool jump(uint64 idx) const;
			bool close();
			bool flush();
			bool clear();
			const nstring& filepath() const;
			char operator[](size_t index) const;


		private:
			nstring m_filepath;
			FILE* m_file;
		public:
			/*************************************[ 公开静态函数 ]******************************************/
			/*文件属性*/
			enum FileAttr
			{
				// 存档
				FA_ARCHIVE,
				// 压缩
				FA_COMPRESSED,
				// 加密
				FA_ENCRYPTED,
				// 隐藏
				FA_HIDDEN,
				// 只读
				FA_READONLY,
				// 系统文件
				FA_SYSTEM,
				// 临时文件
				FA_TEMPORARY,
				// 未知
				FA_NONE
			};
			/***************************************
			 * Struct：文件(目录)信息
			 ***************************************/
			struct FileConf
			{
				FileConf()
				{
					size = 0;
					lastAccessTime = 0;
					lastWriteTime = 0;
					createTime = 0;
					fileattr = FA_NONE;
					isDir = false;
				}
				//文件大小
				uint64 size;
				//最后访问时间
				uint64 lastAccessTime;
				//最后修改时间
				uint64 lastWriteTime;
				//创建时间
				uint64 createTime;
				//属性
				FileAttr fileattr;
				//为目录
				bool isDir;
			};
			/*取文件信息*/
			static bool details(const nstring& filepath, FileConf& fileconf);
			/*取文件名*/
			static bool size(const nstring& filepath, uint64* size);
			/*文件或目录是否存在*/
			static bool exist(const nstring& filepath);
			/*删除文件*/
			static bool remove(const nstring& filepath); 
			/*取目录下所有*/
			static bool list(const nstring& rootPath, std::map<nstring, bool/*文件夹*/>& list);
			/*写数据*/
			static bool write(const nstring& filepath, const char* data, uint32 len);
			static bool write(const nstring& filepath, const buffer& data);
			static bool write(const nstring& filepath, const nstring& data);
			/*读数据*/
			static bool read(const nstring& filepath, buffer& data, open_type type = OT_RW_C);
            static newobj::buffer read(const nstring& filepath,open_type type = OT_R);
			/*创建目录*/
			static bool create_dir(const nstring& filepath,bool recursion = false /*递归创建*/);
			/*复制目录*/
			static bool copy_dir(const nstring& src, const nstring& dst, bool cover = false);
			/*复制文件*/
			static bool copy_file(const nstring& src, const nstring& dst,bool cover = true);
			/*删除目录*/
			static bool remove_dir(const nstring& dirpath,bool recycle = false);
			/*取上级目录*/
			static nstring pre_dir(const nstring& path);
			/*取扩展名*/
			static inline void ext(const nstring& path,nstring& ext)
			{
					int32 idx = path.rfind('.');
					t_ret(idx == -1);
					int32 sublen = path.length() - idx - 1;
					ext.append(path.c_str()+path.length()-sublen,sublen);
			}
			/*取文件名*/
			static nstring filename(const nstring& path);
			/*
				遍历所有文件及目录
				@depth : 1=一级；2=二级；-1=无限 
			*/
			static bool ergodic_all(const nstring& path,std::list<nstring>& dirs,std::list<nstring>& files,int32 depth = -1);
			/*格式化*/
			static nstring format(const nstring& path,bool trailshash = false/*尾部斜杠*/);
            /*格式化斜杠*/
            static nstring format_slash(const nstring& path);
		};
		/*****************************************************************************************
		 * Class：临时文件
		 ****************************************************************************************/
		class NEWOBJ_API tempfile:public file
		{
		public:
			/*****************************************************************************************
			 * Param
			 *			filepath				：				打开或创建的文件。析构后会自动删除该文件
			 ****************************************************************************************/
			tempfile(const nstring& filepath);
			~tempfile();

		};
}
#endif
