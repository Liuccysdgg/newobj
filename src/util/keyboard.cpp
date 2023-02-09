#include "keyboard.h"
#ifdef _WIN32
#include <Windows.h>
void newobj::keyboard::press(uint32 key, uint32 function_key)
{
	keybd_event(function_key,0,0,0);
	keybd_event(key, 0, 0, 0);

	keybd_event(function_key, 0, 2, 0);
	keybd_event(key, 0, 2, 0);
}
#endif

