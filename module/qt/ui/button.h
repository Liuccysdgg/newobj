#pragma once
#include "public/define.h"
#ifdef LIB_QT
#ifdef QT4
#include "Qt/qglobal.h"
#else
#include "QtCore/qglobal.h"
#endif
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include "QtWidgets/qpushbutton.h"
#include <QtCore/qvariant.h>
#else
#include "Qt/qpushbutton.h"
#endif
#include "public/define.h"
#include "basetype/string.h"
#include "util/map.hpp"

#include <functional>
#include "public/extra_data.hpp"
namespace newobj
{
	namespace ui
	{

		class MQT_API button:public QPushButton,public extra_data
		{
		public:
			button(QWidget* parent = nullptr);
			~button();
			void on_clicked(std::function<void(button*)> callback);
			void on_mouse_over(std::function<void(button*)> callback);
			void on_mouse_out(std::function<void(button*)> callback);
			void click();
		protected:
			void enterEvent(QEvent* evt);
			void leaveEvent(QEvent* evt);
			void mousePressEvent(QMouseEvent* e);
		private:
			std::function<void(button*)> m_clicked_callback;
			std::function<void(button*)> m_over_callback;
			std::function<void(button*)> m_out_callback;
		};

	}
}
#endif