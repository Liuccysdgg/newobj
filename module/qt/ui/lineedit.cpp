#include "lineedit.h"
#ifdef LIB_QT
newobj::ui::lineedit::lineedit(QWidget* parent) :QLineEdit(parent)
{
	m_clicked_callback = nullptr;
	m_over_callback = nullptr;
	m_out_callback = nullptr;
}

newobj::ui::lineedit::~lineedit()
{
}
void newobj::ui::lineedit::on_clicked(std::function<void(lineedit*)> callback)
{
	this->m_clicked_callback = callback;
}
void newobj::ui::lineedit::on_mouse_over(std::function<void(lineedit*)> callback)
{
	this->m_over_callback = callback;
}
void newobj::ui::lineedit::on_mouse_out(std::function<void(lineedit*)> callback)
{
	this->m_out_callback = callback;
}
void newobj::ui::lineedit::on_focus_in_event(std::function<void(lineedit*, QFocusEvent*)> callback)
{
	m_focus_in_callback = callback;
}
void newobj::ui::lineedit::on_focus_out_event(std::function<void(lineedit*, QFocusEvent*)> callback)
{
	m_focus_out_callback = callback;
}
void newobj::ui::lineedit::enterEvent(QEvent* evt)
{
	if (this->m_over_callback != nullptr)
		this->m_over_callback(this);
	QLineEdit::enterEvent(evt);
}
void newobj::ui::lineedit::leaveEvent(QEvent* evt)
{
	if (this->m_out_callback != nullptr)
		this->m_out_callback(this);
	QLineEdit::leaveEvent(evt);
}
void newobj::ui::lineedit::mousePressEvent(QMouseEvent* e)
{
	if (this->m_clicked_callback != nullptr)
		this->m_clicked_callback(this);
	QLineEdit::mousePressEvent(e);
}
void newobj::ui::lineedit::focusInEvent(QFocusEvent* event)
{
	::QLineEdit::focusInEvent(event);
	if (this->m_focus_in_callback != nullptr)
		this->m_focus_in_callback(this,event);
}
void newobj::ui::lineedit::focusOutEvent(QFocusEvent* event)
{
	::QLineEdit::focusInEvent(event);
	if (this->m_focus_out_callback != nullptr)
		this->m_focus_out_callback(this, event);
}
#endif