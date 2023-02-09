#include "comobox.h"
#ifdef LIB_QT
newobj::ui::combobox::combobox(QWidget* parent):QComboBox(parent)
{
}

newobj::ui::combobox::~combobox()
{
}

void newobj::ui::combobox::on_focus_in_event(std::function<void(ui::combobox*, QFocusEvent*)> callback)
{
	m_focus_in_callback = callback;
}

void newobj::ui::combobox::on_focus_out_event(std::function<void(ui::combobox*, QFocusEvent*)> callback)
{
	m_focus_out_callback = callback;
}

void newobj::ui::combobox::focusInEvent(QFocusEvent* event)
{
	::QComboBox::focusInEvent(event);
	if (this->m_focus_in_callback != nullptr)
		this->m_focus_in_callback(this, event);
}

void newobj::ui::combobox::focusOutEvent(QFocusEvent* event)
{
	::QComboBox::focusInEvent(event);
	if (this->m_focus_out_callback != nullptr)
		this->m_focus_out_callback(this, event);
}
#endif