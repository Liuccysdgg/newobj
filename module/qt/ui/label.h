#pragma once
#include "public/define.h"
#ifdef LIB_QT
#ifdef QT4
#include "Qt/qglobal.h"
#else
#include "QtCore/qglobal.h"
#endif
#ifdef QT5
#include "QtWidgets/qpushbutton.h"
#include "QtWidgets/qlabel.h"
#include <QtCore/qvariant.h>
#else
#include "Qt/qlabel.h"
#endif
#include "public/define.h"
#include "basetype/string.h"
#include "util/map.hpp"

#include <functional>
#include "public/extra_data.hpp"
#include "public/buffer.h"
namespace newobj
{
	namespace ui
	{

		class MQT_API label :public QLabel,public extra_data
		{
		public:
			label(QWidget* parent = nullptr);
			~label();
			void on_clicked(std::function<void(label*)> callback);
			void on_release(std::function<void(label*)> callback);
			void on_mouse_over(std::function<void(label*)> callback);
			void on_mouse_out(std::function<void(label*)> callback);

			 
			void set_background(const newobj::buffer& data);
		protected:
			void enterEvent(QEvent* evt);
			void leaveEvent(QEvent* evt);
			void mousePressEvent(QMouseEvent* e);
			void mouseReleaseEvent(QMouseEvent* e);
		private:
			std::function<void(label*)> m_clicked_callback;
			std::function<void(label*)> m_release_callback;
			std::function<void(label*)> m_over_callback;
			std::function<void(label*)> m_out_callback;
		};

	}
}
#endif