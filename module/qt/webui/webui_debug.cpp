#include "webui_debug.h"
#if LIB_QT == 1 && !defined(QT_STATIC) && !defined(MSVC_2010)
#include "QtWidgets/qlayout.h"
#include "ui/button.h"
#include "webui_page.h"
newobj::webui::debug::debug(webui::page* page)
{
	m_page = page;

	m_debug = new QWidget();
	m_debug->hide();
	QVBoxLayout* layout = new QVBoxLayout();
	m_view = new QWebEngineView();
	ui::button* debug_btn = new ui::button();
	ui::button* refresh_btn = new ui::button();
	refresh_btn->setText(QTSTR("刷新"));
	layout->addWidget(debug_btn);
	layout->addWidget(refresh_btn);
	layout->addWidget(m_view);
	m_debug->setLayout(layout);
	refresh_btn->on_clicked([&](ui::button*) {
		m_page->view()->reload();
		});
	m_debug->setWindowFlags(Qt::WindowStaysOnTopHint);
	debug_btn->setText(QTSTR("调试组件"));
	debug_btn->setFixedSize(300, 30);
	debug_btn->on_clicked([&](ui::button*) {
		m_view->load(QUrl("http://127.0.0.1:7777"));
		m_view->show();
	});
}

newobj::webui::debug::~debug()
{
	delete m_debug;
}

void newobj::webui::debug::show()
{
	m_debug->show();
}
void newobj::webui::debug::hide()
{
	m_debug->hide();
}
#endif