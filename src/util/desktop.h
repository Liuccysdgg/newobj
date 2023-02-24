#pragma once
#ifdef _WIN32
#include "basetype/string.h"
#include "public/define.h"
#include "util/system.h"
#include "util/process.h"
#include <vector>
#if LIB_QT == 1
#ifdef QT5
#include "QtGui/qpixmap.h"
#endif
#endif
namespace newobj
{
#ifdef _WIN32
		/*********************************************************************************
		 * Class：桌面
		 *********************************************************************************/
		class NEWOBJ_API desktop
		{
		public:
			desktop(const nstring& name,bool create_new,bool destory = true);
			desktop(ptr handle,bool destory = true);
			~desktop();
			/*显示桌面*/
			bool show();
			/*执行程序*/
			bool exec(const nstring& filepath, const nstring& path = "");
			/*置为本桌面*/
			void current();
			/*关闭所有创建进程*/
			void destoryAllProcesses(bool subproc = true);
			/*取句柄*/
			ptr handle();
#if LIB_QT == 1 && defined(QT5)
			/*截图*/
			QPixmap screenshot();
#endif
			/*取桌面句柄*/
			ptr winhandle();
		private:
			/*结束子进程*/
			void desktorySubProcesses(const std::list<newobj::process::ProcData>& list, ptr pid);
		private:
			// 桌面句柄
			ptr m_desktop;
			// 桌面名称
			nstring m_name;
			// 执行程序句柄
			std::vector<uint64> m_exec_hds;
			// 销毁句柄
			bool m_destory;
		public:
			/*************************************[ 公开静态函数 ]******************************************/
			struct DesktopInfo
			{
				DesktopInfo()
				{
					handle = 0;
				}
				nstring name;
				ptr handle;
			};
			/*取所有桌面*/
			static std::vector<DesktopInfo> all(const std::string& winsta);
			/*取当前桌面*/
			static ptr get_current();
			/*显示桌面*/
			static void show(ptr handle);
			/*名称取桌面*/
			static ptr get_desktop(const nstring& name);
			/*关闭桌面*/
			static bool close(ptr desktop);
		};
#endif
}
#endif
