#include "file.h"
#if LIB_QT == 1
#ifdef QT4
#include "Qt/qgraphicseffect.h"
#include "QtCore/qfileinfo.h"
#include "QtCore/qdir.h"
#include "Qt/qdirmodel.h"
#else
#include "QtCore/qstring.h"
#include "QtCore/qfileinfo.h"
#include "QtWidgets/qdirmodel.h"
#endif
QPixmap newobj::ui::get_icon(const QString& path, uint32 width, uint32 height)
{
	QFileInfo fileInfo(path);
	QFileIconProvider icon;
	return icon.icon(fileInfo).pixmap(width, height);
}
QByteArray MQT_API newobj::ui::read(const QString& path)
{
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	return file.readAll();
}
#endif