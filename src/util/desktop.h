#pragma once
#ifdef _WIN32
#include "basetype/string.h"
#include "public/define.h"
#include "util/system.h"
#include "util/process.h"
#include <vector>
#ifdef LIB_QT
#ifdef QT5
#include "QtGui/qpixmap.h"
#endif
#endif
namespace newobj
{
#ifdef _WIN32
		/*********************************************************************************
		 * Class������
		 *********************************************************************************/
		class NEWOBJ_API desktop
		{
		public:
			desktop(const nstring& name,bool create_new,bool destory = true);
			desktop(ptr handle,bool destory = true);
			~desktop();
			/*��ʾ����*/
			bool show();
			/*ִ�г���*/
			bool exec(const nstring& filepath, const nstring& path = "");
			/*��Ϊ������*/
			void current();
			/*�ر����д�������*/
			void destoryAllProcesses(bool subproc = true);
			/*ȡ���*/
			ptr handle();
#if defined(LIB_QT) && defined(QT5)
			/*��ͼ*/
			QPixmap screenshot();
#endif
			/*ȡ������*/
			ptr winhandle();
		private:
			/*�����ӽ���*/
			void desktorySubProcesses(const std::list<newobj::process::ProcData>& list, ptr pid);
		private:
			// ������
			ptr m_desktop;
			// ��������
			nstring m_name;
			// ִ�г�����
			std::vector<uint64> m_exec_hds;
			// ���پ��
			bool m_destory;
		public:
			/*************************************[ ������̬���� ]******************************************/
			struct DesktopInfo
			{
				DesktopInfo()
				{
					handle = 0;
				}
				nstring name;
				ptr handle;
			};
			/*ȡ��������*/
			static std::vector<DesktopInfo> all(const std::string& winsta);
			/*ȡ��ǰ����*/
			static ptr get_current();
			/*��ʾ����*/
			static void show(ptr handle);
			/*����ȡ����*/
			static ptr get_desktop(const nstring& name);
			/*�ر�����*/
			static bool close(ptr desktop);
		};
#endif
}
#endif
