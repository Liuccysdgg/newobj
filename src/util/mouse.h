#pragma once
#include "public/define.h"
#ifdef _WIN32
#include "util/window.h"
namespace newobj
{
	namespace mouse
	{
		/*
			����
			@type ��1 = ������������2 = ����Ҽ�������3 = ��������ס���ţ�
		*/
		void click(ptr_win window,uint32 x,uint32 y,uint32 type);
		/*
			�ƶ�
			@window	:  �ɿ�
		*/
		bool move(ptr_win window, uint32 x, uint32 y);
	}
}
#endif