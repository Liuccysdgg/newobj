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
			// ֻ��
			OT_R,
			// ��д+����
			OT_RW_C
		}; 
		/*****************************************************************************************
		 * Class���ļ�������
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
			/*************************************[ ������̬���� ]******************************************/
			/*�ļ�����*/
			enum FileAttr
			{
				// �浵
				FA_ARCHIVE,
				// ѹ��
				FA_COMPRESSED,
				// ����
				FA_ENCRYPTED,
				// ����
				FA_HIDDEN,
				// ֻ��
				FA_READONLY,
				// ϵͳ�ļ�
				FA_SYSTEM,
				// ��ʱ�ļ�
				FA_TEMPORARY,
				// δ֪
				FA_NONE
			};
			/***************************************
			 * Struct���ļ�(Ŀ¼)��Ϣ
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
				//�ļ���С
				uint64 size;
				//������ʱ��
				uint64 lastAccessTime;
				//����޸�ʱ��
				uint64 lastWriteTime;
				//����ʱ��
				uint64 createTime;
				//����
				FileAttr fileattr;
				//ΪĿ¼
				bool isDir;
			};
			/*ȡ�ļ���Ϣ*/
			static bool details(const nstring& filepath, FileConf& fileconf);
			/*ȡ�ļ���*/
			static bool size(const nstring& filepath, uint64* size);
			/*�ļ���Ŀ¼�Ƿ����*/
			static bool exist(const nstring& filepath);
			/*ɾ���ļ�*/
			static bool remove(const nstring& filepath); 
			/*ȡĿ¼������*/
			static bool list(const nstring& rootPath, std::map<nstring, bool/*�ļ���*/>& list);
			/*д����*/
			static bool write(const nstring& filepath, const char* data, uint32 len);
			static bool write(const nstring& filepath, const buffer& data);
			static bool write(const nstring& filepath, const nstring& data);
			/*������*/
			static bool read(const nstring& filepath, buffer& data, open_type type = OT_RW_C);
            static newobj::buffer read(const nstring& filepath,open_type type = OT_R);
			/*����Ŀ¼*/
			static bool create_dir(const nstring& filepath,bool recursion = false /*�ݹ鴴��*/);
			/*����Ŀ¼*/
			static bool copy_dir(const nstring& src, const nstring& dst, bool cover = false);
			/*�����ļ�*/
			static bool copy_file(const nstring& src, const nstring& dst,bool cover = true);
			/*ɾ��Ŀ¼*/
			static bool remove_dir(const nstring& dirpath,bool recycle = false);
			/*ȡ�ϼ�Ŀ¼*/
			static nstring pre_dir(const nstring& path);
			/*ȡ��չ��*/
			static nstring ext(const nstring& path);
			/*ȡ�ļ���*/
			static nstring filename(const nstring& path);
			/*
				���������ļ���Ŀ¼
				@depth : 1=һ����2=������-1=���� 
			*/
			static bool ergodic_all(const nstring& path,std::list<nstring>& dirs,std::list<nstring>& files,int32 depth = -1);
			/*��ʽ��*/
			static nstring format(const nstring& path,bool trailshash = false/*β��б��*/);
            /*��ʽ��б��*/
            static nstring format_slash(const nstring& path);
		};
		/*****************************************************************************************
		 * Class����ʱ�ļ�
		 ****************************************************************************************/
		class NEWOBJ_API tempfile:public file
		{
		public:
			/*****************************************************************************************
			 * Param
			 *			filepath				��				�򿪻򴴽����ļ�����������Զ�ɾ�����ļ�
			 ****************************************************************************************/
			tempfile(const nstring& filepath);
			~tempfile();

		};
}
#endif
