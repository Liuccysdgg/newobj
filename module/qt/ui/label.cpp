#include "label.h"
#if LIB_QT == 1
newobj::ui::label::label(QWidget* parent) :QLabel(parent)
{
	m_clicked_callback = nullptr;
	m_over_callback = nullptr;
	m_out_callback = nullptr;
}

newobj::ui::label::~label()
{
}
void newobj::ui::label::on_clicked(std::function<void(label*)> callback)
{
	this->m_clicked_callback = callback;
}
void newobj::ui::label::on_release(std::function<void(label*)> callback)
{
}
void newobj::ui::label::on_mouse_over(std::function<void(label*)> callback)
{
	this->m_over_callback = callback;
}
void newobj::ui::label::on_mouse_out(std::function<void(label*)> callback)
{
	this->m_out_callback = callback;
}
void newobj::ui::label::set_background(const newobj::buffer& data)
{
	QImage* img = new QImage;
	img->loadFromData((uchar*)data.data(),data.length());
	QImage scaledimg;
	scaledimg = img->scaled(this->width(), this->height(), Qt::KeepAspectRatio);
	this->setPixmap(QPixmap::fromImage(scaledimg));
}
void newobj::ui::label::enterEvent(QEvent* evt)
{
	if (this->m_over_callback != nullptr)
		this->m_over_callback(this);
}
void newobj::ui::label::leaveEvent(QEvent* evt)
{
	if (this->m_out_callback != nullptr)
		this->m_out_callback(this);
}
void newobj::ui::label::mousePressEvent(QMouseEvent* e)
{
	if (this->m_clicked_callback != nullptr)
		this->m_clicked_callback(this);

}
void newobj::ui::label::mouseReleaseEvent(QMouseEvent* e)
{
	if (this->m_release_callback != nullptr)
		this->m_release_callback(this);
}
#endif