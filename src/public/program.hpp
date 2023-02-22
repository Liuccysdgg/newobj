#pragma once
#include <functional>
#include "public/define.h"
#include "basetype/string.h"
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
	* @ ����ṹ��
	* @ desc�����ڳ����ʼ��
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
		* @ ��ʼ������
		* @ desc��QT��Ŀ���ʼ�� QApplication��������Ҫ��ǰ��ʼ������� initialization();
		*/
		virtual  bool init(int argc, char* argv[]) = 0;
		// ���ٳ���
		void quit(int main_return = 0) 
		{ 
			if (m_init == false)
			{
				printf("���������� initialization ��ɳ�ʼ��");
				abort();
			}
			m_main_return = main_return;
#if LIB_QT == 1
			QApplication::quit();
#else
			m_destory = true; 
#endif
		}
		// ִ�� (����main����β������)
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
		// ��ʼ������,�����ֶ����á�
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
		// �Ƿ�����
		bool m_destory;
		// main��������ֵ
		int m_main_return;
		// �Ƿ��ѳ�ʼ��
		bool m_init;
	};
}