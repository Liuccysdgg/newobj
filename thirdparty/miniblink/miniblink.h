#pragma once
#ifndef MSVC_2010
#include "public/define.h"
#include "public/string.h"
#include "wke.h"
#include "util/window.h"
#include "public/struct.h"
#define BIND_FUNCTION(NAME,BODY,PARAM_COUNT)																								\
	miniblink->bind(NAME, [](jsExecState es, void* param)->jsValue BODY, this, PARAM_COUNT);

#define BIND_FUNCTION2(PTR,NAME,BODY,PARAM_COUNT)																								\
	miniblink->bind(NAME, [](jsExecState es, void* param)->jsValue BODY, PTR, PARAM_COUNT);


#define ARG_INT(IDX) jsToInt(es,jsArg(es,IDX))
#define ARG_BOOL(IDX) jsToBoolean(es,jsArg(es,IDX))
#define ARG_STRING(IDX) jsToString(es,jsArg(es,IDX))

#define JS_UNDEFINED jsUndefined()
#define JS_INT(PARAM) jsInt(PARAM)
#define JS_BOOL(PARAM) jsBoolean(PARAM)
#define JS_STRING(PARAM) jsString(es,PARAM)
class QWidget;
namespace newobj
{
    namespace noui
    {
        class indev;
    }
    namespace browser
    {
        class miniblink
        {
        public: 
            miniblink(const Geometry& geometry,bool dialog);
            ~miniblink(); 

            void set_title(const nstring& title);
            void show(bool show);
            /*回调*/
            void on_load_url_begin(wkeLoadUrlBeginCallback callback);
            void on_load_url_end(wkeLoadUrlEndCallback callback);
            void on_load_finish(wkeLoadingFinishCallback callback);
            void on_window_close(wkeWindowClosingCallback callback);
            void on_window_destroy(wkeWindowDestroyCallback callback);
            void on_document_ready(wkeDocumentReadyCallback callback);
            void on_title_changed(wkeTitleChangedCallback callback);
            void on_create_view(wkeCreateViewCallback callback);
            void on_navigation(wkeNavigationCallback callback);
            void on_load_failed(wkeLoadUrlFailCallback callback);
            ptr_win handle();
            void load(const nstring& url);
            void back();
            void reload();
            void window(bool show);
            void run(const nstring& js);
            void bind(const nstring& funname, wkeJsNativeFunction fun_ptr, void* param = nullptr, unsigned int argCount = 0);
            /*设置UserAgent协议头*/
            void user_agent(const nstring& user_agent);
            /*移到中间*/
            void move_center();
            /*移动*/
            void move(const newobj::Geometry& geometry);
            /*重制尺寸*/
            void resize(const newobj::Size& geometry);
            /*置代理*/
            void set_http_proxy(const nstring& ipaddress,uint32 port);
            /*源码*/
            nstring source();
            /*清除Cookie*/
            void clear_cookie();
            nstring url();

            /*禁止跨域检查*/
            void csp(bool enable);
            /*debug*/
            void debug(bool enable, const nstring& front_end);
            /*qt窗口*/
            QWidget* qwidget();
            /*Dialog 展示*/
            void exec();
        private:
            bool m_dialog;
            wkeWebView m_webView;
            QWidget* m_qwidget;
            newobj::noui::indev* m_noui_indev;
        };


    }
}
#endif