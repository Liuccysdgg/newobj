#pragma once
#include "public/define.h"

#ifdef LIB_QT
#include "QtGui/qpixmap.h"
namespace newobj
{
	namespace ui
	{
		/************************************************************************************
		* Function��ȡ�ļ�ͼ��
		* Param
		*		path					��			·��
		*		width				:			���
		*		height				��			�߶�
		************************************************************************************/
		QPixmap MQT_API get_icon(const QString& path, uint32 width, uint32 height);
	}
}
#endif