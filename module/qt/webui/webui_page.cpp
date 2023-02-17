#include "webui_page.h"
#if LIB_QT == 1 && !defined(QT_STATIC)
#include "webui/webui_callback.hpp"
#include "ui/widget.hpp"
#include "util/codec.h"
#include "util/file.h"
#include "QtWebEngineWidgets/qwebengineview.h"
#include "QtWebChannel/qwebchannel.h"
#include "QtWebEngineWidgets/qwebengineprofile.h"
#include "QtWebEngineCore/qwebenginecookiestore.h"
#include "QtWebEngineWidgets/qwebenginesettings.h"
#include "QtWidgets/qlayout.h"
#include "ui/button.h"
#include "webui_debug.h"
template<typename T>
class w :public ui::widget<T> {
public:
	w<T>(webui::page* page, QWidget* parent = nullptr):ui::widget<T>(parent) { m_page = page; }
	~w<T>() {}
protected:
	void closeEvent(QCloseEvent* event) { m_page->__callback_close_event(event); }
	void resizeEvent(QResizeEvent* event) { m_page->__callback_resize_event(event); }
public:
	webui::page* m_page = nullptr;
};


webui::debug* newobj::webui::page::debug()
{
	if (m_debug == nullptr)
		m_debug = new webui::debug(this);
	return m_debug;
}

newobj::webui::page::page(QWidget* parent, bool dialog)
{
	// ���������
    m_view = new QWebEngineView();
	m_view_channel = new QWebChannel();
	m_view->page()->setWebChannel(m_view_channel);

	// ��������
	{
		m_widget_dialog = dialog;
		if (dialog)
			m_widget = new w<QDialog>(this,parent);
		else
			m_widget = new w<QWidget>(this, parent);
	}
	// �������������
	{
		QHBoxLayout* layout = new QHBoxLayout();
		layout->setMargin(0);
		layout->setSpacing(0);
		layout->addWidget(m_view);
		m_widget->setLayout(layout);
	}

}
newobj::webui::page::~page()
{
	delete m_widget;
	if (m_debug != nullptr)
	{
		delete m_debug;
		m_debug = nullptr;
	}
}
void newobj::webui::page::load(const std::map<nstring,QObject*>& callback)
{
	for_iter(iter, callback)
	{
		m_view_channel->registerObject(iter->first, iter->second);
	}
	m_callbacks = callback;
		
}
QWidget* newobj::webui::page::widget()
{
    return m_widget;
}

QWebEngineView* newobj::webui::page::view()
{
    return m_view;
}

void newobj::webui::page::set_cache_path(const nstring& path)
{
	QWebEngineProfile* engineProfile = view()->page()->profile();
	engineProfile->setCachePath(codec::to_utf8(path));//ָ�����ٻ���·��
	engineProfile->setPersistentStoragePath(path);//ָ���Ự�ͳ־���Cookie·��
	engineProfile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);//�Ự�ͳ־���Cookie�����浽���̻�Ӵ��̻�ԭ
	engineProfile->setHttpCacheType(QWebEngineProfile::DiskHttpCache);//ʹ�ô��̻��档
}

void newobj::webui::page::clear_cache()
{
	QWebEngineProfile* engineProfile = view()->page()->profile();
	engineProfile->clearHttpCache();        // ������
	engineProfile->clearAllVisitedLinks();  // ���������¼
	QWebEngineCookieStore* pCookie = view()->page()->profile()->cookieStore();
	pCookie->deleteAllCookies();            // ����cookie
	pCookie->deleteSessionCookies();       // ����Ựcookie
	newobj::file::remove_dir(engineProfile->persistentStoragePath(),false);
}

void newobj::webui::page::switch_cache(bool open_or_close)
{
	view()->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, open_or_close);
}
#endif