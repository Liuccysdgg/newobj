#pragma once
#include "public/define.h"
#ifdef LIB_QT
#ifdef QT4
#include "Qt/qglobal.h"
#else
#include "QtCore/qglobal.h"
#endif
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include "QtWidgets/qlabel.h"
#else
#include "Qt/qlabel.h"
#endif
#include "public/define.h"
#include "public/string.h"
#include "util/map.hpp"
#include <QtCore/qvariant.h>
#include <functional>
#include "public/extra_data.hpp"
#include <QtGui/qmovie.h>
namespace newobj
{
	namespace ui
	{

		class MQT_API image:public QLabel,public extra_data
		{
		public:
			/*
			 * filepath					：			图片路径
			 * fromImageSize		：			自适应图片大小
			 */
			image(const QString& filepath,bool fromImageSize = true,QWidget* parent = nullptr);
			~image();
			/*置图片*/
			void set(const QString& filepath, bool fromImageSize = true);
			///*置播放速度*/
			//void setSpeed(int32 speed);
		private:
			//QMovie* m_movie = nullptr;
		};

	}
}
#endif