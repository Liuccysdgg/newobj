#include "checkbox.h"

#ifdef LIB_QT
#include "QtGui/qevent.h"
newobj::ui::checkbox::checkbox(QWidget* parent):QCheckBox(parent)
{
	m_clicked_callback = nullptr;
}

newobj::ui::checkbox::~checkbox()
{
}
void newobj::ui::checkbox::mousePressEvent(QMouseEvent* e)
{
	setChecked(!isChecked());
	if (this->m_clicked_callback != nullptr)
		this->m_clicked_callback(this);
	//::QCheckBox::mouseMoveEvent(e);
}
void newobj::ui::checkbox::on_clicked(std::function<void(ui::checkbox*)> callback)
{
	m_clicked_callback = callback;
}
#endif