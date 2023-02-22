#pragma once
#include "public/define.h"
#include "basetype/string.h"
#include <functional>
#if LIB_QT == 1 && !defined(QT_STATIC) && !defined(MSVC_2010)
#include "QtCore/qobject.h"
#include "QtWidgets/qwidget.h"
#include "QtWidgets/qdialog.h"

//#include "QtWebEngineWidgets/qwebengineview.h"

class QWebEngineView;
class QWebChannel;
namespace newobj
{
	/*
	*  @ 页面窗口
	*/
	namespace webui
	{
		class center;
		class callback;
		class debug;
		class page 
		{
		public:
			/*
				@加载回调集
			*/
			void load(const std::map<nstring, QObject*> &callback);
			// 回调类
			template<typename T>
			T* callback(const nstring& name) 
			{
				return (T*)m_callbacks[name];
			}
			// 窗口
			QWidget* widget();
			// 浏览器
			QWebEngineView* view();
			// 指定缓存路径
			void set_cache_path(const nstring& path);
			// 清空缓存
			void clear_cache();
			// 开关缓存
			void switch_cache(bool open_or_close);

			/************************[ 回调 ]***********************/
			// 窗口关闭
			inline void on_close_event(std::function<void(QCloseEvent*)> callback) { __callback_close_event = callback; }
			// 大小改变
			inline void on_resize_event(std::function<void(QResizeEvent*)> callback) { __callback_resize_event = callback; }
			/*DEBUG*/
			webui::debug* debug();
			/*是否为对话框*/
			inline bool dialog() { return m_widget_dialog; }
		private:
			page(QWidget* parent,bool dialog);
			~page();
			friend class webui::center;
			friend class webui::debug;
		private:
			// 显示窗口
			QWidget* m_widget = nullptr;
			// 窗口类型 DIALOG或WIDGET
			bool m_widget_dialog = false;
			// 浏览器
			QWebEngineView* m_view = nullptr;
			QWebChannel* m_view_channel = nullptr;
			// 调试窗口
			webui::debug* m_debug = nullptr;
		public:
			std::function<void(QCloseEvent*)> __callback_close_event;
			std::function<void(QResizeEvent*)> __callback_resize_event;
			std::map<nstring, QObject*> m_callbacks;
		};

	}
}

#endif
