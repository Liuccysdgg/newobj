#pragma once
#include "public/define.h"
#include "public/string.h"

#if LIB_QT == 1 && !defined(QT_STATIC) && !defined(MSVC_2010)
#include "QtWidgets/qwidget.h"
#include "QtWebEngineWidgets/qwebengineview.h"
namespace newobj
{
	/*
	*  @ µ÷ÊÔ´°¿Ú
	*/
	namespace webui
	{
		class page;
		class debug
		{
		public:
			debug(webui::page* page);
			~debug();
			void show();
			void hide();
		private:
			webui::page* m_page = nullptr;
			QWidget* m_debug = nullptr;
			QWebEngineView* m_view = nullptr;
		};
	}
}
#endif