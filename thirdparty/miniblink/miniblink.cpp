#include "miniblink.h"
#include "basetype/string.h"
#include "miniblink.h"
#include "public/exception.h"

#ifndef MSVC_2010
#ifdef LIB_QT
#include "QtWidgets/qwidget.h"
#include "QtWidgets/qdialog.h"
#include "QtGui/qwindow.h"
#include "QtWidgets/qlayout.h"
#include "QtCore/qtimer.h"
#endif
#include "noui/noui_indev.h"
//inline int wkeInitializeEx(const wkeSettings* settings)
//{
//    auto hMod = LoadLibraryA("node.dll");
//    if (hMod) {
//        FN_wkeInitializeEx wkeInitializeExFunc = (FN_wkeInitializeEx)GetProcAddress(hMod, "wkeInitializeEx");
//        wkeInitializeExFunc(settings);
//        WKE_FOR_EACH_DEFINE_FUNCTION(WKE_GET_PTR_ITERATOR0, WKE_GET_PTR_ITERATOR1, WKE_GET_PTR_ITERATOR2, WKE_GET_PTR_ITERATOR3, \
//            WKE_GET_PTR_ITERATOR4, WKE_GET_PTR_ITERATOR5, WKE_GET_PTR_ITERATOR6, WKE_GET_PTR_ITERATOR9, WKE_GET_PTR_ITERATOR10, WKE_GET_PTR_ITERATOR11);
//        return 1;
//    }
//    return 0;
//} 

newobj::browser::miniblink::miniblink(const Geometry& geometry,bool dialog)
{
    m_qwidget = nullptr;
    if (wkeInitializeEx(0) == 0)
    {
        trw_str("³õÊ¼»¯Ê§°Ü,Î´ÕÒµ½ node.dll");
    }
    m_webView = wkeCreateWebWindow(WKE_WINDOW_TYPE_TRANSPARENT, (HWND)0, geometry.x, geometry.y, geometry.width, geometry.height);
    
    QWindow* window = QWindow::fromWinId(handle());
    m_qwidget = QWidget::createWindowContainer(window);
    m_dialog = dialog;
    if (dialog)
    {
        QDialog* dialog = new QDialog;
        QHBoxLayout* layout = new QHBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_qwidget);
        dialog->setLayout(layout);
        m_qwidget = dialog;
    }

    m_qwidget->setGeometry(geometry.x,geometry.y,geometry.width,geometry.height);
    m_qwidget->setGeometry(geometry.x, geometry.y, geometry.width, geometry.height);
    m_noui_indev = new newobj::noui::indev(this);
}
newobj::browser::miniblink::~miniblink()
{
    wkeDestroyWebView(m_webView);
    if (m_qwidget != nullptr)
    {
        if(m_dialog)
            delete (QDialog*)m_qwidget;
        else
            delete m_qwidget;
    }
        
    if(m_noui_indev != nullptr)
        delete m_noui_indev;
    m_qwidget = nullptr;
    m_noui_indev = nullptr;
}

void newobj::browser::miniblink::set_title(const nstring& title)
{
    wkeSetWindowTitle(m_webView,title.c_str());
}

void newobj::browser::miniblink::show(bool show)
{
    wkeShowWindow(m_webView, show);
}

void newobj::browser::miniblink::on_load_url_begin(wkeLoadUrlBeginCallback callback)
{
    wkeOnLoadUrlBegin(m_webView, callback, this);
}

void newobj::browser::miniblink::on_load_url_end(wkeLoadUrlEndCallback callback)
{
    wkeOnLoadUrlEnd(m_webView, callback, this);
    
}

void newobj::browser::miniblink::on_load_finish(wkeLoadingFinishCallback callback)
{
    wkeOnLoadingFinish(m_webView, callback, this);
}

void newobj::browser::miniblink::on_window_close(wkeWindowClosingCallback callback)
{
    wkeOnWindowClosing(m_webView, callback, this);
}

void newobj::browser::miniblink::on_window_destroy(wkeWindowDestroyCallback callback)
{
    wkeOnWindowDestroy(m_webView, callback, this);
}

void newobj::browser::miniblink::on_document_ready(wkeDocumentReadyCallback callback)
{
    wkeOnDocumentReady(m_webView, callback, this);
}

void newobj::browser::miniblink::on_title_changed(wkeTitleChangedCallback callback)
{
    wkeOnTitleChanged(m_webView, callback, this);
}

void newobj::browser::miniblink::on_create_view(wkeCreateViewCallback callback)
{
    wkeOnCreateView(m_webView, callback, this);
}
void newobj::browser::miniblink::on_navigation(wkeNavigationCallback callback)
{
    wkeOnNavigation(m_webView,callback,nullptr);
}
void newobj::browser::miniblink::on_load_failed(wkeLoadUrlFailCallback callback)
{
    wkeOnLoadUrlFail(m_webView, callback,nullptr);
}
ptr_win newobj::browser::miniblink::handle()
{
    return (ptr_win)wkeGetWindowHandle(m_webView);
}
void newobj::browser::miniblink::load(const nstring &url)
{
    wkeLoadURL(m_webView,url.c_str());
}

void newobj::browser::miniblink::back()
{
    if(wkeCanGoBack(m_webView))
        wkeGoBack(m_webView);
}

void newobj::browser::miniblink::reload()
{
    wkeReload(m_webView);
}

//void QMiniblink::resizeEvent(QResizeEvent *event)
//{
//    Q_UNUSED(event);
//    wkeResize(m_webView, this->width(),this->height());
//    QPoint pos=this->mapToGlobal(QPoint(0,0));
//    wkeMoveWindow(m_webView,pos.x(),pos.y(),this->width(),this->height());
//}

//void newobj::browser::miniblink::move()
//{
//    //MoveWindow();
//    /*QPoint pos=this->mapToGlobal(QPoint(0,0));
//    wkeMoveWindow(m_webView,pos.x(),pos.y(),this->width(),this->height());*/
//}

void newobj::browser::miniblink::window(bool show)
{
    wkeShowWindow(m_webView,show);
}
void newobj::browser::miniblink::run(const nstring &js)
{
    wkeRunJS(m_webView,js.c_str());
}

void newobj::browser::miniblink::bind(const nstring &funname, wkeJsNativeFunction fun_ptr, void *param, unsigned int argCount)
{
    wkeJsBindFunction(funname.c_str(),fun_ptr,param,argCount);
}

void newobj::browser::miniblink::user_agent(const nstring& user_agent)
{
    wkeSetUserAgent(m_webView, user_agent.c_str());
}

void newobj::browser::miniblink::move_center()
{
    wkeMoveToCenter(m_webView);
}

void newobj::browser::miniblink::move(const newobj::Geometry& geometry)
{
    wkeMoveWindow(m_webView,geometry.x,geometry.y,geometry.width,geometry.height);
}

void newobj::browser::miniblink::resize(const newobj::Size &size)
{
    wkeResizeWindow(m_webView,size.width,size.height);
    
    
}

void newobj::browser::miniblink::set_http_proxy(const nstring& ipaddress, uint32 port)
{
    wkeProxy proxy;
    memset(proxy.hostname, 0, 100);
    strcpy_s(proxy.hostname,ipaddress.c_str());
    proxy.port = port;
    proxy.type = WKE_PROXY_HTTP;
    wkeSetProxy(&proxy);
}

nstring newobj::browser::miniblink::source()
{
    return wkeGetSource(m_webView);
}

void newobj::browser::miniblink::clear_cookie()
{
    wkeClearCookie(m_webView);
}

nstring newobj::browser::miniblink::url()
{
    return wkeGetURL(m_webView);
}

void newobj::browser::miniblink::csp(bool enable)
{
    wkeSetCspCheckEnable(m_webView,enable);
}
void newobj::browser::miniblink::debug(bool enable,const nstring& front_end)
{
    wkeSetDebugConfig(m_webView, "showDevTools", front_end.c_str());
}
QWidget* newobj::browser::miniblink::qwidget()
{
    return m_qwidget;
}
void newobj::browser::miniblink::exec()
{
    if (m_dialog)
    {
        QTimer::singleShot(100,[&]() {
            ((QDialog*)m_qwidget)->exec();
        });
        
    }
        
    else
        m_qwidget->show();
}
#endif