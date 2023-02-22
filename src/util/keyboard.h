#pragma once
#include "public/define.h"
#define KEYBOARD_0 48
#define KEYBOARD_1 49
#define KEYBOARD_2 50
#define KEYBOARD_3 51
#define KEYBOARD_4 52
#define KEYBOARD_5 53
#define KEYBOARD_6 54
#define KEYBOARD_7 55
#define KEYBOARD_8 56
#define KEYBOARD_9 57

#define KEYBOARD_A 65
#define KEYBOARD_B 66
#define KEYBOARD_C 67
#define KEYBOARD_D 68
#define KEYBOARD_E 69
#define KEYBOARD_F 70
#define KEYBOARD_G 71
#define KEYBOARD_H 72
#define KEYBOARD_I 73
#define KEYBOARD_J 74
#define KEYBOARD_K 75
#define KEYBOARD_L 76
#define KEYBOARD_M 77
#define KEYBOARD_N 78
#define KEYBOARD_O 79
#define KEYBOARD_P 80
#define KEYBOARD_Q 81
#define KEYBOARD_R 82
#define KEYBOARD_S 83
#define KEYBOARD_T 84
#define KEYBOARD_U 85
#define KEYBOARD_V 86
#define KEYBOARD_W 87
#define KEYBOARD_X 88
#define KEYBOARD_Y 89
#define KEYBOARD_Z 90

#define KEYBOARD_F1 112
#define KEYBOARD_F2 113
#define KEYBOARD_F3 114
#define KEYBOARD_F4 115
#define KEYBOARD_F5 116
#define KEYBOARD_F6 117
#define KEYBOARD_F7 118
#define KEYBOARD_F8 119
#define KEYBOARD_F9 120
#define KEYBOARD_F10 121
#define KEYBOARD_F11 122
#define KEYBOARD_F12 123
#define KEYBOARD_F13 124
#define KEYBOARD_F14 125
#define KEYBOARD_F15 126
#define KEYBOARD_F16 127

#define KEYBOARD_BREAK 3
#define KEYBOARD_BACKSPACE 8
#define KEYBOARD_TAB 9
#define KEYBOARD_ENTER 13
#define KEYBOARD_SHIFT 16
#define KEYBOARD_CTRL 17

namespace newobj
{
	
	namespace keyboard
	{

		/*
			组合按键
			@key					：		普通键		KEYBOARD_
			@function_key		：		功能键		KEYBOARD_
		*/
		void press(uint32 key,uint32 function_key);
	}
}