#pragma once
#include "public/define.h"
#ifdef LIB_QT
#ifdef QT4
#include "Qt/qglobal.h"
#else
#include "QtCore/qglobal.h"
#endif
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include "QtWidgets/qcheckbox.h"
#else

#include "Qt/qcheckbox.h"
#endif
#include "public/define.h"
#include "basetype/string.h"
#include "util/map.hpp"
#include <QtCore/qvariant.h>
#include <functional>
#include "public/extra_data.hpp"
namespace newobj
{
	namespace ui
	{

		class MQT_API checkbox:public QCheckBox,public extra_data
		{
		public:
			checkbox(QWidget* parent = nullptr);
			~checkbox();
			virtual void mousePressEvent(QMouseEvent* e);
			void on_clicked(std::function<void(ui::checkbox*)> callback);
		private:
			std::function<void(ui::checkbox*)> m_clicked_callback;
		};

	}
}
#endif