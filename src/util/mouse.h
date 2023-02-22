#pragma once
#include "public/define.h"
#ifdef _WIN32
#include "util/window.h"
namespace newobj
{
	namespace mouse
	{
		/*
			单击
			@type ：1 = 鼠标左键单击；2 = 鼠标右键单击；3 = 鼠标左键按住不放；
		*/
		void click(ptr_win window,uint32 x,uint32 y,uint32 type);
		/*
			移动
			@window	:  可空
		*/
		bool move(ptr_win window, uint32 x, uint32 y);
	}
}
#endif