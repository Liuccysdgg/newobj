#pragma once
#include "public/define.h"
#ifdef LIB_QT
#ifdef QT4
#include "Qt/qglobal.h"
#else
#include "QtCore/qglobal.h"
#endif
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include "QtWidgets/qradiobutton.h"
#include <QtCore/qvariant.h>
#else

#include "Qt/qradiobutton.h"
#include "Qt/qvariant.h"
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

		class MQT_API radio :public QRadioButton,public extra_data
		{
		public:
			radio(QWidget* parent = nullptr);
			~radio();
			virtual void mousePressEvent(QMouseEvent* e);
			void on_clicked(std::function<void(newobj::ui::radio*)> callback);
		private:
			std::function<void(newobj::ui::radio*)> m_clicked_callback;
		};

	}
}
#endif