#pragma once
#include "public/define.h"
#ifdef LIB_QT

#ifdef QT5
#include "QtWidgets/qlineedit.h"
#include <QtCore/qvariant.h>
#else
#include "qt/qlineedit.h"
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

		class MQT_API lineedit :public QLineEdit,public extra_data
		{
		public:
			lineedit(QWidget* parent = nullptr);
			~lineedit();
			void on_clicked(std::function<void(lineedit*)> callback);
			void on_mouse_over(std::function<void(lineedit*)> callback);
			void on_mouse_out(std::function<void(lineedit*)> callback);
			void on_focus_in_event(std::function<void(lineedit*, QFocusEvent*)> callback);
			void on_focus_out_event(std::function<void(lineedit*, QFocusEvent*)> callback);
		protected:
			void enterEvent(QEvent* evt) override;
			void leaveEvent(QEvent* evt) override;
			void mousePressEvent(QMouseEvent* e) override;
			void focusInEvent(QFocusEvent* event) override;
			void focusOutEvent(QFocusEvent* event) override;
			//void keyPressEvent(QKeyEvent* event) override;
		private:
			std::function<void(lineedit*)> m_clicked_callback;
			std::function<void(lineedit*)> m_over_callback;
			std::function<void(lineedit*)> m_out_callback;
			std::function<void(lineedit*, QFocusEvent*)> m_focus_in_callback;
			std::function<void(lineedit*, QFocusEvent*)> m_focus_out_callback;
		};

	}
}
#endif