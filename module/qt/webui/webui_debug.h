#pragma once
#include "public/define.h"
#include "public/string.h"
#include "QtWidgets/qwidget.h"
#if LIB_QT == 1 && !defined(QT_STATIC)
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