#pragma once
#include "topbar.hpp"
#include "public/extra_data.hpp"
namespace newobj
{
	namespace ui
	{
#if LIB_QT == 1
		template<typename T>
		class widget:public ui::topbar<T>,public extra_data
		{
		public:
			widget<T>(QWidget* parent):ui::topbar<T>(parent)
			{

			}
		};
#endif
	}
}