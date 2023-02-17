#pragma once
#include <functional>
#include "public/define.h"
#include "public/string.h"
#include "public/base.h"
#include "util/system.h" 
#if LIB_QT == 1
#if QT5 ==  1
#include "QtWidgets/qapplication.h"
#endif

#endif
namespace newobj
{
	/*
	* @ 程序结构类
	* @ desc：用于程序初始化
	*/
	class program:public newobj::base
	{
	public:
		program()
		{
			m_init = false;
			m_destory = false;
			m_main_return = 0;
#if LIB_QT == 1
			m_app = nullptr;
#endif
		}
		~program()
		{

		}
		/*
		* @ 初始化代码
		* @ desc：QT项目会初始化 QApplication，故若需要提前初始化则调用 initialization();
		*/
		virtual  bool init(int argc, char* argv[]) = 0;
		// 销毁程序
		void quit(int main_return = 0) 
		{ 
			if (m_init == false)
			{
				printf("程序必须调用 initialization 完成初始化");
				abort();
			}
			m_main_return = main_return;
#if LIB_QT == 1
			QApplication::quit();
#else
			m_destory = true; 
#endif
		}
		// 执行 (用于main函数尾部调用)
		int exec()
		{
#if LIB_QT == 1
			m_app->exec();
			return m_main_return;
#else
			while (!m_destory)
				system::sleep_msec(100);
			return m_main_return;
#endif
		}
	protected:
		// 初始化代码,必须手动调用。
		void initialization(int argc, char* argv[])
		{
			if (m_init == false)
			{
#if LIB_QT == 1
				m_app = new QApplication(argc,argv);
#endif
				m_init = true;
			}
				
		}

	private:
#if LIB_QT == 1
		QApplication* m_app;
#endif
		// 是否销毁
		bool m_destory;
		// main函数返回值
		int m_main_return;
		// 是否已初始化
		bool m_init;
	};
}