#pragma once
#include "public/define.h"
#include "public/buffer.h"
#if defined(QT_CORE_LIB) || LIB_QT == 1
#if QT4 == 1
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
			* Function：阴影
			* Param
			*		widget				：			窗口
			*		radio				：			圆角
			*		offset				：			偏移
			************************************************************************************/
		MQT_API void shadow(QWidget* widget, uint32 radio = 30, uint32 offset = 3);
		/************************************************************************************
		* Function：设置背景图片
		* Param
		*		obj					 :			元素
		*		path					：			路径
		************************************************************************************/
		MQT_API void background(QLabel* obj, const QString& filepath);
		MQT_API void background(QLabel* obj, const newobj::buffer& data);
		MQT_API void background(QPushButton* obj, const newobj::buffer& data);
		MQT_API void background(QPushButton* obj, const QString& filepath);
		/************************************************************************************
		* Function：居中
		* Param
		*		parent				：			父
		*		obj					：			对象
		************************************************************************************/
		MQT_API void center(QWidget* parent,QWidget* obj);
		/************************************************************************************
		* Function：设置CSS
		* Param
		*		obj					：			对象
		*		path					：			资源路径
		************************************************************************************/
		MQT_API void setqss(QWidget* obj, const QString& path);
		/************************************************************************************
		* Function：设置图片大小
		* Param
		*		path					：			图片路径
		*		path					：			新图片路径
		*		width				：			宽度
		*		height				：			高度
		************************************************************************************/
		MQT_API bool setsize(const QString& path, const QString new_path, uint32 width, uint32 height);
		MQT_API bool image_radius(const QString& path, const QString new_path, uint32 radius);
		
	}
}
#endif