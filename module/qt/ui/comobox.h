#pragma once
#include "public/define.h"
#ifdef LIB_QT
#ifdef QT4
#include "Qt/qglobal.h"
#else
#include "QtCore/qglobal.h"
#endif
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include "QtWidgets/qcombobox.h"
#else

#include "Qt/qcombobox.h"
#endif
#include "public/define.h"
#include "public/string.h"
#include "util/map.hpp"
#include <QtCore/qvariant.h>
#include <functional>
#include "public/extra_data.hpp"
namespace newobj
{
	namespace ui
	{

		class MQT_API combobox:public QComboBox,public extra_data
		{
		public:
			combobox(QWidget* parent = nullptr);
			~combobox();
			void on_focus_in_event(std::function<void(ui::combobox*, QFocusEvent*)> callback);
			void on_focus_out_event(std::function<void(ui::combobox*, QFocusEvent*)> callback);
		protected:
			void focusInEvent(QFocusEvent* event) override;
			void focusOutEvent(QFocusEvent* event) override;

		private:
			std::function<void(ui::combobox*, QFocusEvent*)> m_focus_in_callback;
			std::function<void(ui::combobox*, QFocusEvent*)> m_focus_out_callback;
		};

	}
}
#endif