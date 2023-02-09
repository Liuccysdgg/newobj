#include "image.h"
#ifdef LIB_QT
newobj::ui::image::image(const QString& filepath, bool fromImageSize,QWidget* parent):QLabel(parent)
{
	//m_movie = new QMovie;
	//this->setMovie(m_movie);

	set(filepath, fromImageSize);

	
}

newobj::ui::image::~image()
{
	//delete m_movie;
}

void newobj::ui::image::set(const QString& filepath, bool fromImageSize)
{
	QImage img;
	img.load(filepath); //将图像资源载入对象img，注意路径，可点进图片右键复制路径
	if (fromImageSize)
	{
		this->setFixedHeight(img.height());
		this->setFixedWidth(img.width());
	}
	this->setPixmap(QPixmap::fromImage(img).scaled(this->geometry().size(), Qt::KeepAspectRatio));
}
//
//void newobj::ui::Image::setSpeed(int32 speed)
//{
//	m_movie->setSpeed(speed);
//}
#endif