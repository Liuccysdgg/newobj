#pragma once
#include "public/define.h"
#include "basetype/string.h"
#include "util/map.hpp"
#if LIB_QT == 1 && !defined(QT_STATIC) && !defined(MSVC_2010)
#include "QtWidgets/qwidget.h"
namespace newobj
{
	/*
	*  @ ����QWebEngine�������UI��
	*/
	namespace webui
	{
		class page;
		class center 
		{
		public:
			/*��������*/
			webui::page* create(const nstring& name,QWidget* parent = nullptr,bool dialog = false);
			/*���ٴ���*/
			void destory(const nstring& name);
			/*��ȡ����*/
			webui::page* get(const nstring& name);

		private:
			newobj::map<nstring, webui::page*> m_pages;
		};

	}
}
#endif