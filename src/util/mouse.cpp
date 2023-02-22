#include "mouse.h"
#ifdef _WIN32
#include <Windows.h>

void newobj::mouse::click(ptr_win window, uint32 x, uint32 y, uint32 type)
{
	type = type == 0 ? 1 : type;
	if (type > 0 && type < 4)
	{
		uint32 idx = 0;
		if (type == 1)
			idx = 6;
		else if (type == 2)
				idx = 24;
		else if (type == 3)
			idx = 3;
		mouse::move(window,x,y);
		mouse_event(idx,x,y,0,0);
		mouse::move(window, x, y);
	}
}

bool newobj::mouse::move(ptr_win window, uint32 x, uint32 y)
{
	POINT point;
	point.x = 0;
	point.y = 0;
	if (window != 0)
		ClientToScreen((HWND)window,&point);
	return SetCursorPos(point.x + x,point.y + y);
}

#endif