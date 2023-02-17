#pragma once
#include "public/define.h"
#include "public/string.h"
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
	*  @ ҳ�洰��
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
				@���ػص���
			*/
			void load(const std::map<nstring, QObject*> &callback);
			// �ص���
			template<typename T>
			T* callback(const nstring& name) 
			{
				return (T*)m_callbacks[name];
			}
			// ����
			QWidget* widget();
			// �����
			QWebEngineView* view();
			// ָ������·��
			void set_cache_path(const nstring& path);
			// ��ջ���
			void clear_cache();
			// ���ػ���
			void switch_cache(bool open_or_close);

			/************************[ �ص� ]***********************/
			// ���ڹر�
			inline void on_close_event(std::function<void(QCloseEvent*)> callback) { __callback_close_event = callback; }
			// ��С�ı�
			inline void on_resize_event(std::function<void(QResizeEvent*)> callback) { __callback_resize_event = callback; }
			/*DEBUG*/
			webui::debug* debug();
			/*�Ƿ�Ϊ�Ի���*/
			inline bool dialog() { return m_widget_dialog; }
		private:
			page(QWidget* parent,bool dialog);
			~page();
			friend class webui::center;
			friend class webui::debug;
		private:
			// ��ʾ����
			QWidget* m_widget = nullptr;
			// �������� DIALOG��WIDGET
			bool m_widget_dialog = false;
			// �����
			QWebEngineView* m_view = nullptr;
			QWebChannel* m_view_channel = nullptr;
			// ���Դ���
			webui::debug* m_debug = nullptr;
		public:
			std::function<void(QCloseEvent*)> __callback_close_event;
			std::function<void(QResizeEvent*)> __callback_resize_event;
			std::map<nstring, QObject*> m_callbacks;
		};

	}
}

#endif
