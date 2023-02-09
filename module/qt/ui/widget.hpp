#pragma once
#include "topbar.hpp"
#include "public/extra_data.h"
namespace newobj
{
	namespace ui
	{
#ifdef LIB_QT
		template<typename T>
		class widget:public ui::topbar<T>,public extra_data
		{
		public:
			widget(QWidget* parent):ui::topbar<T>(parent)
			{

			}
		};
#endif
	}
}