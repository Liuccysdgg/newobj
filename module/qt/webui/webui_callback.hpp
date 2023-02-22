#pragma once
#include "public/string.h"
#if LIB_QT == 1 && !defined(QT_STATIC) && !defined(MSVC_2010)
class QObject;
namespace newobj
{
	/*
	*  @ 回调基类
	*/
	namespace webui
	{
	
		class page;
		class center;
		class callback
		{
		public:

			callback(const nstring& name, QObject* obj) { m_obj = obj; m_name = name; m_page = nullptr; }
			~callback() {}
			inline webui::page* webuipage() { return m_page; }

			friend class webui::page;
			friend class webui::center;
		private:
			QObject* m_obj = nullptr;
			webui::page* m_page;
			nstring m_name;
		};
	}
}
#endif