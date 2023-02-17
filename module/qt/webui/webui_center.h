#pragma once
#include "public/define.h"
#include "public/string.h"
#include "util/map.hpp"
#if LIB_QT == 1 && !defined(QT_STATIC) && !defined(MSVC_2010)
#include "QtWidgets/qwidget.h"
namespace newobj
{
	/*
	*  @ 基于QWebEngine的浏览器UI类
	*/
	namespace webui
	{
		class page;
		class center 
		{
		public:
			/*创建窗口*/
			webui::page* create(const nstring& name,QWidget* parent = nullptr,bool dialog = false);
			/*销毁窗口*/
			void destory(const nstring& name);
			/*获取窗口*/
			webui::page* get(const nstring& name);

		private:
			newobj::map<nstring, webui::page*> m_pages;
		};

	}
}
#endif