#pragma once
#include "public/define.h"

#if LIB_QT == 1
#include "QtGui/qpixmap.h"
namespace newobj
{
	namespace ui
	{
		/************************************************************************************
		* Function：取文件图标
		* Param
		*		path					：			路径
		*		width				:			宽度
		*		height				：			高度
		************************************************************************************/
		QPixmap MQT_API get_icon(const QString& path, uint32 width, uint32 height);
		/************************************************************************************
		* Function：读资源
		* Desc：可以是应用资源也可以是文件系统资源
		* Param
		*		path					：			路径
		************************************************************************************/
		QByteArray MQT_API read(const QString& path);
	}
}
#endif