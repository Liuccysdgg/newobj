#include "file.h"
#ifdef LIB_QT
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
#endif