#pragma once
#include "public/define.h"

#ifdef LIB_QT
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
	}
}
#endif