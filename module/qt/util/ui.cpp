#include "ui.h"
#ifdef LIB_QT
#ifdef QT4
#include "Qt/qgraphicseffect.h"
#include "QtCore/qfileinfo.h"
#include "QtCore/qdir.h"
#include "Qt/qdirmodel.h"
#include "Qt/qdesktopwidget.h"
#include "qt/qapplication.h"
#include "QtGui/qpainter.h"
#else
#include "QtWidgets/qgraphicseffect.h"
#include "QtWidgets/qapplication.h"
#include "QtWidgets/qdesktopwidget.h"
#include "QtCore/qfile.h"
#include "QtGui/qpainter.h"
#endif

void newobj::ui::shadow(QWidget* widget, uint32 radio, uint32 offset)
{
	QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
	shadow->setBlurRadius(radio);
	shadow->setColor(QColor(0, 0, 0, 20));
	shadow->setOffset(offset);
	widget->setGraphicsEffect(shadow);
	widget->raise();
}

void newobj::ui::background(QLabel* obj, const QString& filepath)
{
	QImage img;
	img.load(filepath); //将图像资源载入对象img，注意路径，可点进图片右键复制路径
	obj->setPixmap(QPixmap::fromImage(img).scaled(obj->geometry().size(), Qt::KeepAspectRatio));
}
void newobj::ui::background(QLabel* obj, const newobj::buffer& data)
{
	QImage img;
	img.loadFromData((uchar*)data.data(),data.length()); //将图像资源载入对象img，注意路径，可点进图片右键复制路径
	obj->setPixmap(QPixmap::fromImage(img).scaled(obj->geometry().size(), Qt::KeepAspectRatio));
}
void newobj::ui::background(QPushButton* obj, const newobj::buffer& data)
{
	QPixmap pixmap;
	pixmap.loadFromData((uchar*)data.data(), data.length());
	QPixmap fitpixmap = pixmap.scaled(obj->width(), obj->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	obj->setIcon(QIcon(fitpixmap));
	obj->setIconSize(QSize(obj->width(), obj->height()));
	obj->setFlat(true);
}
void newobj::ui::background(QPushButton* obj, const QString& filepath)
{
	QPixmap pixmap(filepath);
	QPixmap fitpixmap = pixmap.scaled(obj->width(), obj->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	obj->setIcon(QIcon(fitpixmap));
	obj->setIconSize(QSize(obj->width(), obj->height()));
	obj->setFlat(true);
}

void newobj::ui::center(QWidget* parent, QWidget* obj)
{
	auto src_widget = obj->width();
	auto src_height = obj->height();
	auto geo = obj->geometry();
	auto desktopGeo = QApplication::desktop()->availableGeometry();

	QPoint zPoint;
	if (parent == nullptr) {
		zPoint.setX(desktopGeo.x() + desktopGeo.width() / 2);
		zPoint.setY(desktopGeo.y() + desktopGeo.height() / 2);
	}
	else {
		zPoint.setX(parent->x() + parent->width() / 2);
		zPoint.setY(parent->y() + parent->height() / 2);
	}


	geo.setWidth(src_widget);
	geo.setHeight(src_height);
	geo.setX(zPoint.x() - src_widget / 2);
	geo.setY(zPoint.y() - src_height / 2);
	obj->setGeometry(geo);
}

void newobj::ui::setqss(QWidget* obj, const QString& path)
{
	QFile file(path);
	file.open(QFile::ReadOnly);
	obj->setStyleSheet(file.readAll().data());
}

bool newobj::ui::setsize(const QString& path, const QString new_path, uint32 width, uint32 height)
{
	QImage img;
	f_ret_f(img.load(path));
	if (img.width() == width && img.height() == height)
	{
		return img.save(new_path);
	}
	QPixmap pix = QPixmap::fromImage(img).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
	return pix.save(new_path);
}

bool newobj::ui::image_radius(const QString& path, const QString new_path, uint32 radius)
{
	// 压缩图片
	QPixmap pixmap;
	pixmap.load(path);
	QPixmap dest(2 * radius, 2 * radius);
	dest.fill(Qt::transparent);
	QPainter painter(&dest);
	// 抗锯齿 + 平滑边缘处理
	painter.setRenderHints(QPainter::Antialiasing, true);
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	// 裁剪为圆角
	QPainterPath path2;
	path2.addEllipse(0, 0, 2 * radius, 2 * radius);
	painter.setClipPath(path2);
	painter.drawPixmap(0, 0, 2 * radius, 2 * radius, pixmap);
	return dest.save(new_path);
}
#endif