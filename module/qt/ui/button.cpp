#include "button.h"
#if LIB_QT == 1
newobj::ui::button::button(QWidget* parent) :QPushButton(parent)
{
	m_clicked_callback = nullptr;
	m_over_callback = nullptr;
	m_out_callback = nullptr;
}

newobj::ui::button::~button()
{
}
void newobj::ui::button::on_clicked(std::function<void(button*)> callback)
{
	this->m_clicked_callback = callback;
}
void newobj::ui::button::on_mouse_over(std::function<void(button*)> callback)
{
	this->m_over_callback = callback;
}
void newobj::ui::button::on_mouse_out(std::function<void(button*)> callback)
{
	this->m_out_callback = callback;
}
void newobj::ui::button::click()
{
	mousePressEvent(nullptr);
}
void newobj::ui::button::enterEvent(QEvent* evt)
{
	if (this->m_over_callback != nullptr)
		this->m_over_callback(this);
}
void newobj::ui::button::leaveEvent(QEvent* evt)
{
	if (this->m_out_callback != nullptr)
		this->m_out_callback(this);
}
void newobj::ui::button::mousePressEvent(QMouseEvent* e)
{
	if (this->m_clicked_callback != nullptr)
		this->m_clicked_callback(this);
}
#endif