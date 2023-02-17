#include "webui_center.h"
#if LIB_QT == 1 && !defined(QT_STATIC) && !defined(MSVC_2010)
#include "webui/webui_page.h"
#include "QtWidgets/qlayout.h"

webui::page* newobj::webui::center::create(const nstring& name, QWidget* parent, bool dialog)
{
	webui::page* page = new webui::page(parent,dialog);
	m_pages.set(name, page,true);
	return page;
}

void newobj::webui::center::destory(const nstring& name)
{
	webui::page* page = nullptr;
	if (m_pages.get(name, page))
		delete page;
	m_pages.del(name);
}

webui::page* newobj::webui::center::get(const nstring& name)
{
	webui::page* page = nullptr;
	m_pages.get(name, page);
	return page;
}


#endif