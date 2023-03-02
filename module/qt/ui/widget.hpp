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
			void on_mouse_over(std::function<void(ui::widget<T>*)> callback)
			{
				this->m_over_callback = callback;
			}
			void on_mouse_out(std::function<void(ui::widget<T>*)> callback)
			{
				this->m_out_callback = callback;
			}
		protected:
			void enterEvent(QEvent* evt)
			{
				if (this->m_over_callback != nullptr)
					this->m_over_callback(this);
			}
			void leaveEvent(QEvent* evt)
			{
				if (this->m_out_callback != nullptr)
					this->m_out_callback(this);
			}
		private:
			std::function<void(ui::widget<T>*)> m_over_callback;
			std::function<void(ui::widget<T>*)> m_out_callback;
		};
#endif
	}
}