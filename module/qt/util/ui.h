#pragma once
#include "public/define.h"
#include "public/buffer.h"
#if defined(QT_CORE_LIB) || LIB_QT == 1
#ifdef QT4
#include "Qt/qwidget.h"
#include "Qt/qlabel.h"
#include "Qt/qpushbutton.h"
#else
#include "QtWidgets/qwidget.h"
#include "QtWidgets/qlabel.h"
#include "QtWidgets/qpushbutton.h"
#endif

namespace newobj
{
	namespace ui
	{
		/************************************************************************************
			* Function����Ӱ
			* Param
			*		widget				��			����
			*		radio				��			Բ��
			*		offset				��			ƫ��
			************************************************************************************/
		MQT_API void shadow(QWidget* widget, uint32 radio = 30, uint32 offset = 3);
		/************************************************************************************
		* Function�����ñ���ͼƬ
		* Param
		*		obj					 :			Ԫ��
		*		path					��			·��
		************************************************************************************/
		MQT_API void background(QLabel* obj, const QString& filepath);
		MQT_API void background(QLabel* obj, const newobj::buffer& data);
		MQT_API void background(QPushButton* obj, const newobj::buffer& data);
		MQT_API void background(QPushButton* obj, const QString& filepath);
		/************************************************************************************
		* Function������
		* Param
		*		parent				��			��
		*		obj					��			����
		************************************************************************************/
		MQT_API void center(QWidget* parent,QWidget* obj);
		/************************************************************************************
		* Function������CSS
		* Param
		*		obj					��			����
		*		path					��			��Դ·��
		************************************************************************************/
		MQT_API void setqss(QWidget* obj, const QString& path);
		/************************************************************************************
		* Function������ͼƬ��С
		* Param
		*		path					��			ͼƬ·��
		*		path					��			��ͼƬ·��
		*		width				��			���
		*		height				��			�߶�
		************************************************************************************/
		MQT_API bool setsize(const QString& path, const QString new_path, uint32 width, uint32 height);
		MQT_API bool image_radius(const QString& path, const QString new_path, uint32 radius);
		
	}
}
#endif