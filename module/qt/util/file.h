#pragma once
#include "public/define.h"

#if LIB_QT == 1
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
		/************************************************************************************
		* Function������Դ
		* Desc��������Ӧ����ԴҲ�������ļ�ϵͳ��Դ
		* Param
		*		path					��			·��
		************************************************************************************/
		QByteArray MQT_API read(const QString& path);
	}
}
#endif