#include "radio.h"
#if LIB_QT == 1
#include "QtGui/qevent.h"

newobj::ui::radio::radio(QWidget* parent):QRadioButton(parent)
{
}

newobj::ui::radio::~radio()
{
}

void newobj::ui::radio::mousePressEvent(QMouseEvent* e)
{
	setChecked(!isChecked());
	if (this->m_clicked_callback != nullptr)
		this->m_clicked_callback(this);
}

void newobj::ui::radio::on_clicked(std::function<void(ui::radio*)> callback)
{
	m_clicked_callback = callback;
}
#endif